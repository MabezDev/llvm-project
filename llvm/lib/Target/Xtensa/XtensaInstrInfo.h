//===-- XtensaInstrInfo.h - Xtensa Instruction Information ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Xtensa implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_XTENSA_XTENSAINSTRINFO_H
#define LLVM_LIB_TARGET_XTENSA_XTENSAINSTRINFO_H

#include "Xtensa.h"
#include "XtensaRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_INSTRINFO_HEADER

#include "XtensaGenInstrInfo.inc"

namespace llvm {

class XtensaTargetMachine;
class XtensaSubtarget;
class XtensaInstrInfo : public XtensaGenInstrInfo {
  const XtensaRegisterInfo RI;
  XtensaSubtarget &STI;

public:
  XtensaInstrInfo(XtensaSubtarget &STI);

  void adjustStackPtr(unsigned SP, int64_t Amount, MachineBasicBlock &MBB,
                      MachineBasicBlock::iterator I) const;

  // Return the XtensaRegisterInfo, which this class owns.
  const XtensaRegisterInfo &getRegisterInfo() const { return RI; }

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI,
                   const DebugLoc &DL, unsigned DestReg, unsigned SrcReg,
                   bool KillSrc) const override;
  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI, unsigned SrcReg,
                           bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI, unsigned DestReg,
                            int FrameIdx, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;

  // Get the load and store opcodes for a given register class and offset.
  void getLoadStoreOpcodes(const TargetRegisterClass *RC, unsigned &LoadOpcode,
                           unsigned &StoreOpcode, int64_t offset) const;

  // Emit code before MBBI in MI to move immediate value Value into
  // physical register Reg.
  void loadImmediate(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI,
                     unsigned *Reg, int64_t Value) const;
};
} // end namespace llvm

#endif /* LLVM_LIB_TARGET_XTENSA_XTENSAINSTRINFO_H */
