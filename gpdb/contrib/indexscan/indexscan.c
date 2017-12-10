#include "postgres.h"

#include "fmgr.h"
#include "funcapi.h"
#include "miscadmin.h"

#include "access/appendonlytid.h"
#include "access/genam.h"
#include "access/nbtree.h"
#include "utils/builtins.h"
#include "utils/rel.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(readindex);
Datum readindex(PG_FUNCTION_ARGS);

typedef struct _readindexinfo
{
	AttrNumber	outattnum;
	Relation	irel;
	Relation	hrel;
	int			num_pages;
	BlockNumber	blkno;
	Buffer		buf;
	Page		page;
	BTPageOpaque opaque;
	OffsetNumber offnum;
	OffsetNumber minoff;
	OffsetNumber maxoff;
} readindexinfo;

static bool
FileRepPrimary_IsResyncManagerOrWorker()
{
	return false;
}

#define FIXED_COLUMN 5

static text *
istatus_text(ItemId itemid)
{
	StringInfoData	buf;

	initStringInfo(&buf);

	if (ItemIdDeleted(itemid))
		appendStringInfoString(&buf, "DELETED ");
	if (ItemIdIsNormal(itemid))
		appendStringInfoString(&buf, "USED ");
	if (ItemIdIsDead(itemid))
		appendStringInfoString(&buf, "DEAD ");

	if (buf.len == 0)
		appendStringInfoString(&buf, "UNUSED ");

	buf.data[buf.len - 1] = '\0';
	return cstring_to_text(buf.data);
}

static text *
hstatus_text(HeapTupleHeader tuple, bool visible)
{
	StringInfoData	buf;

	initStringInfo(&buf);

	if (!visible)
		appendStringInfoString(&buf, "NOT_VISIBLE ");
	if (tuple->t_infomask & HEAP_XMIN_COMMITTED)
		appendStringInfoString(&buf, "XMIN_COMITTTED ");
	if (tuple->t_infomask & HEAP_XMIN_INVALID)
		appendStringInfoString(&buf, "XMIN_INVALID ");
	if (tuple->t_infomask & HEAP_XMAX_COMMITTED)
		appendStringInfoString(&buf, "XMAX_COMITTTED ");
	if (tuple->t_infomask & HEAP_XMAX_INVALID)
		appendStringInfoString(&buf, "XMAX_INVALID ");
	if (tuple->t_infomask & HEAP_XMAX_IS_MULTI)
		appendStringInfoString(&buf, "XMAX_IS_MULTI ");
	if (tuple->t_infomask & HEAP_UPDATED)
		appendStringInfoString(&buf, "UPDATED ");
	if (tuple->t_infomask & HEAP_MOVED_OFF)
		appendStringInfoString(&buf, "MOVED_OFF ");
	if (tuple->t_infomask & HEAP_MOVED_IN)
		appendStringInfoString(&buf, "MOVED_IN ");

	if (buf.len == 0)
		appendStringInfoString(&buf, "NONE ");

	buf.data[buf.len - 1] = '\0';
	return cstring_to_text(buf.data);
}

static void
readindextuple(readindexinfo *info, Datum *values, bool *nulls)
{
	BlockNumber blkno;
	Page		page;
	OffsetNumber offnum;
	IndexTuple	itup;
	HeapTupleData	htup;
	Buffer		hbuf;
	AttrNumber	attno;
	TupleDesc	tupdesc = RelationGetDescr(info->irel);

	blkno = info->blkno;
	page = info->page;
	offnum = info->offnum;
	itup = (IndexTuple) PageGetItem(page, PageGetItemId(page, offnum));
	htup.t_self = itup->t_tid;

	values[1] = ItemPointerGetDatum(&itup->t_tid);

	if (info->hrel == NULL)
		values[2] = PointerGetDatum(cstring_to_text(AOTupleIdToString((AOTupleId *)&itup->t_tid))); 
	else
		values[2] = PointerGetDatum(cstring_to_text("N/A"));

	values[3] = PointerGetDatum(istatus_text(PageGetItemId(page, offnum)));

	if (info->hrel != NULL)
	{
		if (heap_fetch(info->hrel, SnapshotAny, &htup, &hbuf, true, NULL))
			values[4] = PointerGetDatum(hstatus_text(htup.t_data, true));
		else if (htup.t_data)
			values[4] = PointerGetDatum(hstatus_text(htup.t_data, false));
		else
			values[4] = PointerGetDatum(cstring_to_text("NOT_FOUND"));

		ReleaseBuffer(hbuf);
	}
	else
		values[4] = PointerGetDatum(cstring_to_text("N/A"));

	for (attno = 1; attno <= tupdesc->natts; attno++)
	{
		bool		isnull;

		values[FIXED_COLUMN+attno-1] = index_getattr(itup, attno, tupdesc, &isnull);
		nulls[FIXED_COLUMN+attno-1] = isnull;
	}

}

Datum
readindex(PG_FUNCTION_ARGS)
{
	FuncCallContext	   *funcctx;
	readindexinfo	   *info;

	MIRROREDLOCK_BUFMGR_DECLARE;

	if (SRF_IS_FIRSTCALL())
	{
		Oid		irelid = PG_GETARG_OID(0);
		TupleDesc	tupdesc;
		MemoryContext oldcontext;
		AttrNumber		outattnum;
		Relation	irel;
		TupleDesc	itupdesc;
		int			i;
		AttrNumber	attno;

		irel = index_open(irelid, AccessShareLock);
		itupdesc = RelationGetDescr(irel);
		outattnum = FIXED_COLUMN + itupdesc->natts;

		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
		tupdesc = CreateTemplateTupleDesc(outattnum, false);
		attno = 1;
		TupleDescInitEntry(tupdesc, attno++, "ictid", TIDOID, -1, 0);
		TupleDescInitEntry(tupdesc, attno++, "hctid", TIDOID, -1, 0);
		TupleDescInitEntry(tupdesc, attno++, "aotid", TEXTOID, -1, 0);
		TupleDescInitEntry(tupdesc, attno++, "istatus", TEXTOID, -1, 0);
		TupleDescInitEntry(tupdesc, attno++, "hstatus", TEXTOID, -1, 0);

		for (i = 0; i < itupdesc->natts; i++)
		{
			Form_pg_attribute attr = itupdesc->attrs[i];
			TupleDescInitEntry(tupdesc, attno++, NameStr(attr->attname), attr->atttypid, attr->atttypmod, 0);
		}

		funcctx->tuple_desc = BlessTupleDesc(tupdesc);
		info = (readindexinfo *) palloc(sizeof(readindexinfo));
		funcctx->user_fctx = (void *) info;

		info->outattnum = outattnum;
		info->irel = irel;
		info->hrel = relation_open(irel->rd_index->indrelid, AccessShareLock);
		if (info->hrel->rd_rel != NULL &&
				(info->hrel->rd_rel->relstorage == 'a' ||
				 info->hrel->rd_rel->relstorage == 'c'))
		{
			relation_close(info->hrel, AccessShareLock);
			info->hrel = NULL;
		}
		info->num_pages = RelationGetNumberOfBlocks(irel);
		info->blkno = BTREE_METAPAGE + 1;
		info->page = NULL;

		MemoryContextSwitchTo(oldcontext);
	}

	funcctx = SRF_PERCALL_SETUP();
	info = (readindexinfo *) funcctx->user_fctx;

	while (info->blkno < info->num_pages)
	{
		Datum		values[255];
		bool		nulls[255];
		ItemPointerData		itid;
		HeapTuple	tuple;
		Datum		result;

		if (info->page == NULL)
		{
			MIRROREDLOCK_BUFMGR_LOCK;
			info->buf = ReadBuffer(info->irel, info->blkno);
			info->page = BufferGetPage(info->buf);
			info->opaque = (BTPageOpaque) PageGetSpecialPointer(info->page);
			info->minoff = P_FIRSTDATAKEY(info->opaque);
			info->maxoff = PageGetMaxOffsetNumber(info->page);
			info->offnum = info->minoff;
			MIRROREDLOCK_BUFMGR_UNLOCK;
		}
		if (!P_ISLEAF(info->opaque) || info->offnum > info->maxoff)
		{
			ReleaseBuffer(info->buf);
			info->page = NULL;
			info->blkno++;
			continue;
		}

		MemSet(nulls, false, info->outattnum * sizeof(bool));

		ItemPointerSet(&itid, info->blkno, info->offnum);
		values[0] = ItemPointerGetDatum(&itid);
		readindextuple(info, values, nulls);

		info->offnum = OffsetNumberNext(info->offnum);

		tuple = heap_form_tuple(funcctx->tuple_desc, values, nulls);
		result = HeapTupleGetDatum(tuple);
		SRF_RETURN_NEXT(funcctx, result);
	}

	if (info->hrel != NULL)
		relation_close(info->hrel, AccessShareLock);
	index_close(info->irel, AccessShareLock);
	SRF_RETURN_DONE(funcctx);
}
