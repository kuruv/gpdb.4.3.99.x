/*-------------------------------------------------------------------------
 *
 * pg_user_mapping.h
 *	  definition of the system "user mapping" relation (pg_user_mapping)
 *
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/catalog/pg_user_mapping.h,v 1.3 2009/06/11 14:49:10 momjian Exp $
 *
 * NOTES
 *	  the genbki.sh script reads this file and generates .bki
 *	  information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_USER_MAPPING_H
#define PG_USER_MAPPING_H

#include "catalog/genbki.h"

/* TIDYCAT_BEGINFAKEDEF

   CREATE TABLE pg_user_mapping
   with (camelcase=UserMapping, relid=2895, reltype_oid=6449)
   (
   umuser     oid, 
   umserver   oid, 
   umoptions  text[]
   );

   create unique index on pg_user_mapping(oid) with (indexid=3316, CamelCase=UserMappingOid, syscacheid=USERMAPPINGOID, syscache_nbuckets=128);

   create unique index on pg_user_mapping(umuser, umserver) with (indexid=3317, CamelCase=UserMappingUserServer, syscacheid=USERMAPPINGUSERSERVER, syscache_nbuckets=128);

   alter table pg_user_mapping add fk umuser on pg_authid(oid);
   alter table pg_user_mapping add fk umserver on pg_foreign_server(oid);

   TIDYCAT_ENDFAKEDEF
*/

/* ----------------
 *		pg_user_mapping definition.  cpp turns this into
 *		typedef struct FormData_pg_user_mapping
 * ----------------
 */
#define UserMappingRelationId	2895

CATALOG(pg_user_mapping,2895)
{
	Oid			umuser;			/* Id of the user, InvalidOid if PUBLIC is
								 * wanted */
	Oid			umserver;		/* server of this mapping */

	/*
	 * VARIABLE LENGTH FIELDS start here.  These fields may be NULL, too.
	 */

	text		umoptions[1];	/* user mapping options */
} FormData_pg_user_mapping;

/* ----------------
 *		Form_pg_user_mapping corresponds to a pointer to a tuple with
 *		the format of pg_user_mapping relation.
 * ----------------
 */
typedef FormData_pg_user_mapping *Form_pg_user_mapping;

/* ----------------
 *		compiler constants for pg_user_mapping
 * ----------------
 */

#define Natts_pg_user_mapping				3
#define Anum_pg_user_mapping_umuser			1
#define Anum_pg_user_mapping_umserver		2
#define Anum_pg_user_mapping_umoptions		3

#endif   /* PG_USER_MAPPING_H */
