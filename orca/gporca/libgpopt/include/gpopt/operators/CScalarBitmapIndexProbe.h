//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2014 Pivotal, Inc.
//
//	@filename:
//		CScalarBitmapIndexProbe.h
//
//	@doc:
//		Bitmap index probe scalar operator
//
//	@owner:
//		
//
//	@test:
//
//---------------------------------------------------------------------------

#ifndef GPOPT_CScalarBitmapIndexProbe_H
#define GPOPT_CScalarBitmapIndexProbe_H

#include "gpos/base.h"

#include "gpopt/operators/CScalar.h"

namespace gpopt
{
	// fwd declarations
	class CColRefSet;
	class CIndexDescriptor;

	//---------------------------------------------------------------------------
	//	@class:
	//		CScalarBitmapIndexProbe
	//
	//	@doc:
	//		Bitmap index probe scalar operator
	//
	//---------------------------------------------------------------------------
	class CScalarBitmapIndexProbe : public CScalar
	{
		private:
			// index descriptor
			CIndexDescriptor *m_pindexdesc;

			// bitmap type id
			IMDId *m_pmdidBitmapType;

			// private copy ctor
			CScalarBitmapIndexProbe(const CScalarBitmapIndexProbe &);

		public:
			// ctor
			CScalarBitmapIndexProbe
				(
				IMemoryPool *pmp,
				CIndexDescriptor *pindexdesc,
				IMDId *pmdidBitmapType
				);

			// ctor
			// only for transforms
			explicit
			CScalarBitmapIndexProbe(IMemoryPool *pmp);

			// dtor
			virtual
			~CScalarBitmapIndexProbe();

			// index descriptor
			CIndexDescriptor *Pindexdesc() const
			{
				return m_pindexdesc;
			}

			// bitmap type id
			virtual
			IMDId *PmdidType() const
			{
				return m_pmdidBitmapType;
			}

			// identifier
			virtual
			EOperatorId Eopid() const
			{
				return EopScalarBitmapIndexProbe;
			}

			// return a string for operator name
			virtual
			const CHAR *SzId() const
			{
				return "CScalarBitmapIndexProbe";
			}

			// operator specific hash function
			virtual
			ULONG UlHash() const;

			// match function
			virtual
			BOOL FMatch(COperator *pop) const;

			// sensitivity to order of inputs
			virtual
			BOOL FInputOrderSensitive() const
			{
				return false;
			}

			// return a copy of the operator with remapped columns
			virtual
			COperator *PopCopyWithRemappedColumns
						(
						IMemoryPool *, //pmp,
						HMUlCr *, //phmulcr,
						BOOL //fMustExist
						)
			{
				return PopCopyDefault();
			}

			// debug print
			virtual
			IOstream &OsPrint(IOstream &) const;

			// conversion
			static
			CScalarBitmapIndexProbe *PopConvert
				(
				COperator *pop
				)
			{
				GPOS_ASSERT(NULL != pop);
				GPOS_ASSERT(EopScalarBitmapIndexProbe == pop->Eopid());

				return dynamic_cast<CScalarBitmapIndexProbe *>(pop);
			}

	};  // class CScalarBitmapIndexProbe
}

#endif // !GPOPT_CScalarBitmapIndexProbe_H

// EOF
