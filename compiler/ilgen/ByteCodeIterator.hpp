/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef BYTECODEITERATOR_INCL
#define BYTECODEITERATOR_INCL

#include <stdint.h>                 // for int32_t, uint16_t, uint32_t
#include "compile/Compilation.hpp"  // for Compilation
#include "infra/Link.hpp"           // for TR_Link

class TR_Memory;
namespace TR { class Block; }
namespace TR { class ResolvedMethodSymbol; }


template <typename ByteCode, typename ResolvedMethod> class TR_ByteCodeIterator
   {
public:
   TR_ByteCodeIterator(TR::ResolvedMethodSymbol *resolvedMethodSym, ResolvedMethod *m, TR::Compilation *comp)
      : _methodSymbol(resolvedMethodSym),
        _method(m),
        _compilation(comp),
        _trMemory(comp->trMemory()),
        _maxByteCodeIndex(m->maxBytecodeIndex())
      { }

   struct TryCatchInfo : TR_Link<TryCatchInfo>
      {
      void initialize(uint16_t s, uint16_t e, uint16_t h, uint32_t c)
         {
         _startIndex = s;
         _endIndex = e;
         _handlerIndex = h;
         _catchType = c;
         }
      bool operator==(TryCatchInfo & o)
         {
         return _handlerIndex == o._handlerIndex && _catchType == o._catchType;
         }

      uint16_t      _startIndex;
      uint16_t      _endIndex;
      uint16_t      _handlerIndex;
      uint32_t      _catchType;
      TR::Block    * _firstBlock;
      TR::Block    * _lastBlock;
      TR::Block    * _catchBlock;
      };

   TR::Compilation *comp()                const       { return _compilation; }
   TR_Memory *trMemory()                   const       { return _trMemory; }
   TR::ResolvedMethodSymbol *methodSymbol() const       { return _methodSymbol; }
   ResolvedMethod          *method()       const       { return _method; }

   int32_t  bcIndex()                      const       { return _bcIndex; }
   void     setIndex(int32_t i)                        { _bcIndex = i; }

   int32_t  maxByteCodeIndex()             const       { return _maxByteCodeIndex; }



   // Abstract Methods - subclasses must provide these
   ByteCode first();
   ByteCode next();
   ByteCode current();

protected:
   // Abstract Methods - subclasses must provide these
   void printByteCodePrologue();
   void printByteCode();
   void printByteCodeEpilogue();

   bool isBranch();
   int32_t branchDesination(int32_t base);

   TR::ResolvedMethodSymbol * _methodSymbol;
   ResolvedMethod          * _method;
   TR::Compilation        * _compilation;
   TR_Memory               * _trMemory;
   int32_t             const _maxByteCodeIndex;


   int32_t                   _bcIndex;
   };

#endif

