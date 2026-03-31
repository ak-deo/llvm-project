//===-- MyLivenessAnalysis.h ------------------*- C++ -*-===//

#ifndef LLVM_TRANSFORMS_UTILS_MYLIVENESSANALYSIS_H
#define LLVM_TRANSFORMS_UTILS_MYLIVENESSANALYSIS_H

#include "llvm/IR/PassManager.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/SetOperations.h"

namespace llvm {

using LivenessMap = DenseMap<const Instruction*, DenseSet<Value*>>;
  
class MyLivenessAnalysisPass : public PassInfoMixin<MyLivenessAnalysisPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

private:
  void printLivenessMap(LivenessMap &LM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_MYLIVENESSANALYSIS_H
