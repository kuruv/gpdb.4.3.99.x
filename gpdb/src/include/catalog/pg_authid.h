/*-------------------------------------------------------------------------
 *
 * pg_authid.h
 *	  definition of the system "authorization identifier" relation (pg_authid)
 *	  along with the relation's initial contents.
 *
 *	  pg_shadow and pg_group are now publicly accessible views on pg_authid.
 *
 *
 * Copyright (c) 2006-2010, Greenplum inc.
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/catalog/pg_authid.h,v 1.5 2006/03/05 15:58:54 momjian Exp $
 *
 * NOTES
 *	  the genbki.sh script reads this file and generates .bki
 *	  information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_AUTHID_H
#define PG_AUTHID_H

#include "catalog/genbki.h"

/* TIDYCAT_BEGINDEF

   CREATE TABLE pg_authid
   with (CamelCase=AuthId, shared=true, relid=1260, toast_oid=2842, toast_index=2843)
   (
   rolname name, -- name of role  
   rolsuper boolean, -- read this field via superuser() only! 
   rolinherit boolean, -- inherit privileges from other roles? 
   rolcreaterole boolean, -- allowed to create more roles? 
   rolcreatedb boolean, -- allowed to create databases? 
   rolcatupdate boolean, -- allowed to alter catalogs manually? 
   rolcanlogin boolean, -- allowed to log in as session user? 
   rolconnlimit integer, -- max connections allowed (-1=no limit) 

   -- remaining fields may be null. use heap_getattr to read them! 
   rolpassword text, -- password, if any 
   rolvaliduntil timestamp with time zone, -- password expiration time, if any
   rolconfig text[], -- GUC settings to apply at login 
   rolresqueue oid, -- ID of resource queue for this role 
   
   -- GP added fields
   rolcreaterextgpfd boolean, -- allowed to create readable gpfdist tbl? 
   rolcreaterexthttp boolean, -- allowed to create readable http tbl? 
   rolcreatewextgpfd boolean, -- allowed to create writable gpfdist tbl? 
   rolcreaterexthdfs boolean, -- allowed to create readable gphdfs tbl?
   rolcreatewexthdfs boolean -- allowed to create writable gphdfs tbl?
   );

   CREATE UNIQUE INDEX on pg_authid(rolname) with (indexid=2676, syscacheid=AUTHNAME, syscache_nbuckets=128);

   CREATE UNIQUE INDEX on pg_authid(oid) with (indexid=2677, syscacheid=AUTHOID, syscache_nbuckets=128);

   CREATE INDEX on pg_authid(rolresqueue) with (CamelCase=AuthIdRolResQueue,indexid=6029);

   alter table pg_authid add fk rolresqueue on pg_resqueue(oid);

   TIDYCAT_ENDDEF
*/
/* TIDYCAT_BEGIN_CODEGEN 

   WARNING: DO NOT MODIFY THE FOLLOWING SECTION: 
   Generated by tidycat.pl version 22.
   on Mon Nov 29 13:34:32 2010
*/


/*
 TidyCat Comments for pg_authid:
  Table is shared, so catalog.c:IsSharedRelation is updated.
  Table has an Oid column.
  Table does not have static type (only legal for pre-3.3 tables). 
  Table has TOASTable columns, but TOAST table does not have static type.
  Table has weird hack for timestamp column.
 
*/

/*
 * The CATALOG definition has to refer to the type of "rolvaliduntil" as
 * "timestamptz" (lower case) so that bootstrap mode recognizes it.  But
 * the C header files define this type as TimestampTz.	Since the field is
 * potentially-null and therefore cannot be accessed directly from C code,
 * there is no particular need for the C struct definition to show the
 * field type as TimestampTz --- instead we just make it Datum.
 */

#define timestamptz Datum

/* ----------------
 *		pg_authid definition.  cpp turns this into
 *		typedef struct FormData_pg_authid
 * ----------------
 */
#define AuthIdRelationId	1260

CATALOG(pg_authid,1260) BKI_SHARED_RELATION
{
	NameData	rolname;			/* name of role   */
	bool		rolsuper;			/* read this field via superuser() only!  */
	bool		rolinherit;			/* inherit privileges from other roles?  */
	bool		rolcreaterole;		/* allowed to create more roles?  */
	bool		rolcreatedb;		/* allowed to create databases?  */
	bool		rolcatupdate;		/* allowed to alter catalogs manually?  */
	bool		rolcanlogin;		/* allowed to log in as session user?  */
	int4		rolconnlimit;		/* max connections allowed (-1=no limit)  */


	/* remaining fields may be null. use heap_getattr to read them!  */
	text		rolpassword;		/* password, if any  */
	timestamptz	rolvaliduntil;		/* password expiration time, if any */
	text		rolconfig[1];		/* GUC settings to apply at login  */
	Oid			rolresqueue;		/* ID of resource queue for this role  */
	/* GP added fields */
	bool		rolcreaterextgpfd;	/* allowed to create readable gpfdist tbl?  */
	bool		rolcreaterexthttp;	/* allowed to create readable http tbl?  */
	bool		rolcreatewextgpfd;	/* allowed to create writable gpfdist tbl?  */
	bool		rolcreaterexthdfs;	/* allowed to create readable gphdfs tbl? */
	bool		rolcreatewexthdfs;	/* allowed to create writable gphdfs tbl? */
} FormData_pg_authid;

#undef timestamptz


/* ----------------
 *		Form_pg_authid corresponds to a pointer to a tuple with
 *		the format of pg_authid relation.
 * ----------------
 */
typedef FormData_pg_authid *Form_pg_authid;


/* ----------------
 *		compiler constants for pg_authid
 * ----------------
 */
#define Natts_pg_authid						17
#define Anum_pg_authid_rolname				1
#define Anum_pg_authid_rolsuper				2
#define Anum_pg_authid_rolinherit			3
#define Anum_pg_authid_rolcreaterole		4
#define Anum_pg_authid_rolcreatedb			5
#define Anum_pg_authid_rolcatupdate			6
#define Anum_pg_authid_rolcanlogin			7
#define Anum_pg_authid_rolconnlimit			8
#define Anum_pg_authid_rolpassword			9
#define Anum_pg_authid_rolvaliduntil		10
#define Anum_pg_authid_rolconfig			11
#define Anum_pg_authid_rolresqueue			12
#define Anum_pg_authid_rolcreaterextgpfd	13
#define Anum_pg_authid_rolcreaterexthttp	14
#define Anum_pg_authid_rolcreatewextgpfd	15
#define Anum_pg_authid_rolcreaterexthdfs	16
#define Anum_pg_authid_rolcreatewexthdfs	17


/* TIDYCAT_END_CODEGEN */

/* ----------------
 *		initial contents of pg_authid
 *
 * The uppercase quantities will be replaced at initdb time with
 * user choices.
 *
 * MPP-7845: add default queue DEFAULTRESQUEUE_OID 6055
 * ----------------
 */
DATA(insert OID = 10 ( "POSTGRES" t t t t t t -1 _null_ _null_ _null_ 6055 t t t t t));

#define BOOTSTRAP_SUPERUSERID 10

#define Schema_pg_authid \
{1260, {"rolname"}          ,   19, -1, NAMEDATALEN,  1, 0, -1, -1, false, 'p' ,'i', true, false, false, true, 0}, \
{1260, {"rolsuper"}         ,   16, -1,           1,  2, 0, -1, -1,  true, 'p' ,'c', true, false, false, true, 0}, \
{1260, {"rolinherit"}       ,   16, -1,           1,  3, 0, -1, -1,  true, 'p' ,'c', true, false, false, true, 0}, \
{1260, {"rolcreaterole"}    ,   16, -1,           1,  4, 0, -1, -1,  true, 'p' ,'c', true, false, false, true, 0}, \
{1260, {"rolcreatedb"}      ,   16, -1,           1,  5, 0, -1, -1,  true, 'p' ,'c', true, false, false, true, 0}, \
{1260, {"rolcatupdate"}     ,   16, -1,           1,  6, 0, -1, -1,  true, 'p' ,'c', true, false, false, true, 0}, \
{1260, {"rolcanlogin"}      ,   16, -1,           1,  7, 0, -1, -1,  true, 'p' ,'c', true, false, false, true, 0}, \
{1260, {"rolconnlimit"}     ,   23, -1,           4,  8, 0, -1, -1,  true, 'p' ,'i', true, false, false, true, 0}, \
{1260, {"rolpassword"}      ,   25, -1,          -1,  9, 0, -1, -1, false, 'x' ,'i',false, false, false, true, 0}, \
{1260, {"rolvaliduntil"}    , 1184, -1,           8, 10, 0, -1, -1,  true, 'p' ,'d',false, false, false, true, 0}, \
{1260, {"rolconfig"}        , 1009, -1,          -1, 11, 1, -1, -1, false, 'x' ,'i',false, false, false, true, 0}, \
{1260, {"rolresqueue"}      ,   26, -1,           4, 12, 0, -1, -1,  true, 'p' ,'i',false, false, false, true, 0}, \
{1260, {"rolcreaterextgpfd"},   16, -1,           1, 13, 0, -1, -1,  true, 'p' ,'c',false, false, false, true, 0}, \
{1260, {"rolcreaterexthttp"},   16, -1,           1, 14, 0, -1, -1,  true, 'p' ,'c',false, false, false, true, 0}, \
{1260, {"rolcreatewextgpfd"},   16, -1,           1, 15, 0, -1, -1,  true, 'p' ,'c',false, false, false, true, 0}, \
{1260, {"rolcreaterexthdfs"},   16, -1,           1, 16, 0, -1, -1,  true, 'p' ,'c',false, false, false, true, 0}, \
{1260, {"rolcreatewexthdfs"},   16, -1,           1, 17, 0, -1, -1,  true, 'p' ,'c',false, false, false, true, 0}
#endif   /* PG_AUTHID_H */