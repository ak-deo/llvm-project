#include "llvm/Transforms/Utils/MyLivenessAnalysis.h"
#include "llvm/IR/Instruction.h"

using namespace llvm;

void MyLivenessAnalysisPass::printLivenessMap(LivenessMap &LM) {
  for (auto &[Inst, Vals] : LM) {
    errs() << "(";
    for (Value* Val: Vals) {
	  errs() << Val->getName() << ", ";
    }
    errs() << ")\n";
  }
}

PreservedAnalyses MyLivenessAnalysisPass::run(Function &F,
                                              FunctionAnalysisManager &AM) {
  // Liveness analysis pass
  errs() << "\n\nLiveness analysis for function: " << F.getName() << "\n";

  // Compute gen and kill sets for every instruction
  LivenessMap GenSet;
  LivenessMap KillSet;
  for (auto &BB : F) {
    for (auto &I : BB) {
      if (auto *PN = dyn_cast<PHINode>(&I)) {
        for (unsigned Idx = 0; Idx < PN->getNumIncomingValues(); ++Idx) {
          Value *IncomingVal = PN->getIncomingValue(Idx);
          const BasicBlock *IncomingBlock = PN->getIncomingBlock(Idx);
          const Instruction *IncomingBlockTerminator =
			IncomingBlock->getTerminator();
		  if (isa<Instruction>(IncomingVal) || isa<Argument>(IncomingVal)) {
			GenSet[IncomingBlockTerminator].insert(IncomingVal);
		  }
        }
      } else {
		// Gen set: Main case
		for (auto &U : I.operands()) {
		  if (isa<Instruction>(U) || isa<Argument>(U)) {
			GenSet[&I].insert(U.get());
		  }
		}
      }
      // Kill set
      KillSet[&I].insert(&I);
	}
  }

  errs() << "DEBUG: Gen sets" << "\n";
  printLivenessMap(GenSet);
  errs() << "DEBUG: Kill sets" << "\n";
  printLivenessMap(KillSet);
  
  // Compute in and out sets for every instruction
  LivenessMap InSet;
  LivenessMap OutSet;
  bool Changed = true;
  while (Changed) {
	Changed = false;
	for (auto &BB : F) {
	  for (auto &I : BB) {
		// Update in set
		auto oldIn = InSet[&I];
        DenseSet<Value *> IS = GenSet[&I];
        set_union(IS, set_difference(OutSet[&I], KillSet[&I]));
        InSet[&I] = IS;

        // Update out set
        auto oldOut = OutSet[&I];
        DenseSet<Value *> OS;
        if (I.isTerminator()) {
          for (const auto &SuccBB : I.successors()) {
            const Instruction &SuccI = *SuccBB->begin();
            set_union(OS, InSet[&SuccI]);
          }
        } else {
		  set_union(OS, InSet[I.getNextNode()]);
        }
		OutSet[&I] = OS;
        Changed |= (InSet[&I] != oldIn) || (OutSet[&I] != oldOut);
	  }
	}
  }

  // Print out all the in sets and out sets

  errs() << "In sets" << "\n";
  printLivenessMap(InSet);
  errs() << "--------------------------------------" << "\n";
  errs() << "Out sets" << "\n";
  printLivenessMap(OutSet);
  // No analyses change, because we did not modify any source code
  return PreservedAnalyses::all();
}
