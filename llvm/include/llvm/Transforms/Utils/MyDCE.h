//===-- MyDCE.h ------------------------------*- C++ -*-===//

#ifndef LLVM_TRANSFORMS_UTILS_MYDCE_H
#define LLVM_TRANSFORMS_UTILS_MYDCE_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class MyDCEPass : public PassInfoMixin<MyDCEPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};
  
} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_MYDCE_H
