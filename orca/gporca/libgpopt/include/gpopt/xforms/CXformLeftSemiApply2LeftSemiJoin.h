//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CXformLeftSemiApply2LeftSemiJoin.h
//
//	@doc:
//		Turn LS apply into LS join
//
//	@owner:
//		
//
//	@test:
//
//
//---------------------------------------------------------------------------
#ifndef GPOPT_CXformLeftSemiApply2LeftSemiJoin_H
#define GPOPT_CXformLeftSemiApply2LeftSemiJoin_H

#include "gpos/base.h"
#include "gpopt/xforms/CXformApply2Join.h"
#include "gpopt/operators/ops.h"


namespace gpopt
{
	using namespace gpos;

	//---------------------------------------------------------------------------
	//	@class:
	//		CXformLeftSemiApply2LeftSemiJoin
	//
	//	@doc:
	//		Transform Apply into Join by decorrelating the inner side
	//
	//---------------------------------------------------------------------------
	class CXformLeftSemiApply2LeftSemiJoin : public CXformApply2Join<CLogicalLeftSemiApply, CLogicalLeftSemiJoin>
	{

		private:

			// private copy ctor
			CXformLeftSemiApply2LeftSemiJoin(const CXformLeftSemiApply2LeftSemiJoin &);

		public:

			// ctor
			explicit
			CXformLeftSemiApply2LeftSemiJoin
				(
				IMemoryPool *pmp
				)
				:
				CXformApply2Join<CLogicalLeftSemiApply, CLogicalLeftSemiJoin>(pmp, true /*fDeepTree*/)
			{}

			// ctor with a passed pattern
			CXformLeftSemiApply2LeftSemiJoin
				(
				IMemoryPool *pmp,
				CExpression *pexprPattern
				)
				:
				CXformApply2Join<CLogicalLeftSemiApply, CLogicalLeftSemiJoin>(pmp, pexprPattern)
			{}

			// dtor
			virtual
			~CXformLeftSemiApply2LeftSemiJoin()
			{}

			// ident accessors
			virtual
			EXformId Exfid() const
			{
				return ExfLeftSemiApply2LeftSemiJoin;
			}

			virtual
			const CHAR *SzId() const
			{
				return "CXformLeftSemiApply2LeftSemiJoin";
			}

			// compute xform promise for a given expression handle
			virtual
			EXformPromise Exfp(CExpressionHandle &exprhdl) const;

			// actual transform
			virtual
			void Transform(CXformContext *pxfctxt, CXformResult *pxfres, CExpression *pexpr) const;


	}; // class CXformLeftSemiApply2LeftSemiJoin

}

#endif // !GPOPT_CXformLeftSemiApply2LeftSemiJoin_H

// EOF
