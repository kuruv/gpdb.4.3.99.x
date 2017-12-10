//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CJobFactory.h
//
//	@doc:
//		Highly concurrent job factory;
//		Uses bulk memory allocation and atomic primitives to
//		create and recycle jobs with minimal sychronization;
//
//	@owner:
//		
//
//	@test:
//
//
//---------------------------------------------------------------------------
#ifndef GPOPT_CJobFactory_H
#define GPOPT_CJobFactory_H

#include "gpos/base.h"
#include "gpos/common/CSyncPool.h"

#include "gpopt/search/CJob.h"
#include "gpopt/search/CJobGroupOptimization.h"
#include "gpopt/search/CJobGroupExpressionOptimization.h"
#include "gpopt/search/CJobGroupExploration.h"
#include "gpopt/search/CJobGroupExpressionExploration.h"
#include "gpopt/search/CJobGroupImplementation.h"
#include "gpopt/search/CJobGroupExpressionImplementation.h"
#include "gpopt/search/CJobTransformation.h"
#include "gpopt/search/CJobTest.h"

namespace gpopt
{
	using namespace gpos;

	//---------------------------------------------------------------------------
	//	@class:
	//		CJobFactory
	//
	//	@doc:
	//		Highly concurrent job factory
	//
	//---------------------------------------------------------------------------
	class CJobFactory
	{
		private:

			// memory pool
			IMemoryPool *m_pmp;

			// number of jobs in each pool
			const ULONG m_ulJobs;

			// container for testing jobs
			CSyncPool<CJobTest> *m_pspjTest;

			// container for group optimization jobs
			CSyncPool<CJobGroupOptimization> *m_pspjGroupOptimization;

			// container for group implementation jobs
			CSyncPool<CJobGroupImplementation> *m_pspjGroupImplementation;

			// container for group exploration jobs
			CSyncPool<CJobGroupExploration> *m_pspjGroupExploration;

			// container for group expression optimization jobs
			CSyncPool<CJobGroupExpressionOptimization> *m_pspjGroupExpressionOptimization;

			// container for group expression implementation jobs
			CSyncPool<CJobGroupExpressionImplementation> *m_pspjGroupExpressionImplementation;

			// container for group expression exploration jobs
			CSyncPool<CJobGroupExpressionExploration> *m_pspjGroupExpressionExploration;

			// container for transformation jobs
			CSyncPool<CJobTransformation> *m_pspjTransformation;

			// retrieve job of specific type
			template<class T>
			T *PtRetrieve
				(
				CSyncPool<T> *&pspt
				)
			{
				if (NULL == pspt)
				{
					pspt = GPOS_NEW(m_pmp) CSyncPool<T>(m_pmp, m_ulJobs);
					pspt->Init(GPOS_OFFSET(T, m_ulId));
				}

				return pspt->PtRetrieve();
			}

			// release job
			template<class T>
			void Release
				(
				T *pt,
				CSyncPool<T> *pspt
				)
			{
				GPOS_ASSERT(NULL != pt);
				GPOS_ASSERT(NULL != pspt);

				pspt->Recycle(pt);
			}

			// truncate job pool
			template<class T>
			void TruncatePool
				(
				CSyncPool<T> *&pspt
				)
			{
				GPOS_DELETE(pspt);
				pspt = NULL;
			}

			// no copy ctor
			CJobFactory(const CJobFactory&);

		public:

			// ctor
			CJobFactory
				(
				IMemoryPool *pmp,
				ULONG ulJobs
				);

			// dtor
			~CJobFactory();

			// create job of specific type
			CJob *PjCreate(CJob::EJobType ejt);

			// release completed job
			void Release(CJob *pj);

			// truncate the container for the specific job type
			void Truncate(CJob::EJobType ejt);

	}; // class CJobFactory

}


#endif // !GPOPT_CJobFactory_H


// EOF
