#include "llvm/Transforms/Utils/MyDCE.h"

namespace llvm {

PreservedAnalyses MyDCEPass::run(Function &F, FunctionAnalysisManager &AM) {
  for (auto &BB : F) {
    for (BasicBlock::iterator I = BB.begin(); I != BB.end(); ++I) {
      // Check if it has side effects
      if (I->mayHaveSideEffects()) {
		continue;
      }
      // If the use list is empty, then the instruction
      // is dead. We can remove it.
      if (I->use_empty()) {
		I = I->eraseFromParent();
      }
    }
  }
  return PreservedAnalyses::none();
}

} // namespace llvm
