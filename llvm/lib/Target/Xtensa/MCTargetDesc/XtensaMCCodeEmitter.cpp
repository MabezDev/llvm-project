//===-- XtensaMCCodeEmitter.cpp - Convert Xtensa Code to Machine Code -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the XtensaMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "mccodeemitter"
#include "MCTargetDesc/XtensaFixupKinds.h"
#include "MCTargetDesc/XtensaMCExpr.h"
#include "MCTargetDesc/XtensaMCTargetDesc.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"

#define GET_INSTRMAP_INFO
#include "XtensaGenInstrInfo.inc"
#undef GET_INSTRMAP_INFO

using namespace llvm;

namespace {
class XtensaMCCodeEmitter : public MCCodeEmitter {
  const MCInstrInfo &MCII;
  MCContext &Ctx;
  bool IsLittleEndian;

public:
  XtensaMCCodeEmitter(const MCInstrInfo &mcii, MCContext &ctx, bool isLE)
      : MCII(mcii), Ctx(ctx), IsLittleEndian(isLE) {}

  ~XtensaMCCodeEmitter() {}

  // OVerride MCCodeEmitter.
  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

private:
  // Automatically generated by TableGen.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  // Called by the TableGen code to get the binary encoding of operand
  // MO in MI.  Fixups is the list of fixups against MI.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getJumpTargetEncoding(const MCInst &MI, unsigned int OpNum,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  unsigned getBranchTargetEncoding(const MCInst &MI, unsigned int OpNum,
                                   SmallVectorImpl<MCFixup> &Fixups,
                                   const MCSubtargetInfo &STI) const;

  unsigned getCallEncoding(const MCInst &MI, unsigned int OpNum,
                           SmallVectorImpl<MCFixup> &Fixups,
                           const MCSubtargetInfo &STI) const;

  unsigned getL32RTargetEncoding(const MCInst &MI, unsigned OpNum,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  unsigned getMemRegEncoding(const MCInst &MI, unsigned OpNo,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getImm8OpValue(const MCInst &MI, unsigned OpNo,
                          SmallVectorImpl<MCFixup> &Fixups,
                          const MCSubtargetInfo &STI) const;

  unsigned getImm8_sh8OpValue(const MCInst &MI, unsigned OpNo,
                              SmallVectorImpl<MCFixup> &Fixups,
                              const MCSubtargetInfo &STI) const;

  unsigned getImm12OpValue(const MCInst &MI, unsigned OpNo,
                           SmallVectorImpl<MCFixup> &Fixups,
                           const MCSubtargetInfo &STI) const;

  unsigned getUimm4OpValue(const MCInst &MI, unsigned OpNo,
                           SmallVectorImpl<MCFixup> &Fixups,
                           const MCSubtargetInfo &STI) const;

  unsigned getUimm5OpValue(const MCInst &MI, unsigned OpNo,
                           SmallVectorImpl<MCFixup> &Fixups,
                           const MCSubtargetInfo &STI) const;

  unsigned getImm1_16OpValue(const MCInst &MI, unsigned OpNo,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getImm1n_15OpValue(const MCInst &MI, unsigned OpNo,
                              SmallVectorImpl<MCFixup> &Fixups,
                              const MCSubtargetInfo &STI) const;

  unsigned getImm32n_95OpValue(const MCInst &MI, unsigned OpNo,
                               SmallVectorImpl<MCFixup> &Fixups,
                               const MCSubtargetInfo &STI) const;

  unsigned getImm8n_7OpValue(const MCInst &MI, unsigned OpNo,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getImm64n_4nOpValue(const MCInst &MI, unsigned OpNo,
                               SmallVectorImpl<MCFixup> &Fixups,
                               const MCSubtargetInfo &STI) const;

  unsigned getEntry_Imm12OpValue(const MCInst &MI, unsigned OpNo,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  unsigned getShimm1_31OpValue(const MCInst &MI, unsigned OpNo,
                               SmallVectorImpl<MCFixup> &Fixups,
                               const MCSubtargetInfo &STI) const;

  unsigned getB4constOpValue(const MCInst &MI, unsigned OpNo,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getB4constuOpValue(const MCInst &MI, unsigned OpNo,
                              SmallVectorImpl<MCFixup> &Fixups,
                              const MCSubtargetInfo &STI) const;
  unsigned getShimmSeimm7_22OpValue(const MCInst &MI, unsigned OpNo,
                                    SmallVectorImpl<MCFixup> &Fixups,
                                    const MCSubtargetInfo &STI) const;
};
} // namespace

MCCodeEmitter *llvm::createXtensaMCCodeEmitter(const MCInstrInfo &MCII,
                                               const MCRegisterInfo &MRI,
                                               MCContext &Ctx) {
  return new XtensaMCCodeEmitter(MCII, Ctx, true);
}

void XtensaMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  uint64_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  unsigned Size = MCII.get(MI.getOpcode()).getSize();

  if (IsLittleEndian) {
    // Little-endian insertion of Size bytes.
    unsigned ShiftValue = 0;
    for (unsigned I = 0; I != Size; ++I) {
      OS << uint8_t(Bits >> ShiftValue);
      ShiftValue += 8;
    }
  } else {
    // TODO Big-endian insertion of Size bytes.
    llvm_unreachable("Big-endian mode currently is not supported!");
  }
}

unsigned
XtensaMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());
  if (MO.isImm()) {
    long res = static_cast<unsigned>(MO.getImm());
    return res;
  }

  llvm_unreachable("Unhandled expression!");
  return 0;
}

unsigned
XtensaMCCodeEmitter::getJumpTargetEncoding(const MCInst &MI, unsigned int OpNum,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNum);

  if (MO.isImm())
    return MO.getImm();

  const MCExpr *Expr = MO.getExpr();
  Fixups.push_back(MCFixup::create(
      0, Expr, MCFixupKind(Xtensa::fixup_xtensa_jump_18), MI.getLoc()));
  return 0;
}

unsigned XtensaMCCodeEmitter::getBranchTargetEncoding(
    const MCInst &MI, unsigned int OpNum, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNum);
  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  const MCExpr *Expr = MO.getExpr();
  switch (MI.getOpcode()) {
  case Xtensa::BEQZ:
  case Xtensa::BGEZ:
  case Xtensa::BLTZ:
  case Xtensa::BNEZ:
    Fixups.push_back(MCFixup::create(
        0, Expr, MCFixupKind(Xtensa::fixup_xtensa_branch_12), MI.getLoc()));
    return 0;
  default:
    Fixups.push_back(MCFixup::create(
        0, Expr, MCFixupKind(Xtensa::fixup_xtensa_branch_8), MI.getLoc()));
    return 0;
  }
}

unsigned
XtensaMCCodeEmitter::getCallEncoding(const MCInst &MI, unsigned int OpNum,
                                     SmallVectorImpl<MCFixup> &Fixups,
                                     const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNum);
  if (MO.isImm()) {
    int32_t Res = MO.getImm();
    if (Res & 0x3) {
      llvm_unreachable("Unexpected operand value!");
    }
    Res >>= 2;
    return Res;
  }

  assert((MO.isExpr()) && "Unexpected operand value!");
  const MCExpr *Expr = MO.getExpr();
  Fixups.push_back(MCFixup::create(
      0, Expr, MCFixupKind(Xtensa::fixup_xtensa_call_18), MI.getLoc()));
  return 0;
}

unsigned
XtensaMCCodeEmitter::getL32RTargetEncoding(const MCInst &MI, unsigned OpNum,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNum);
  if (MO.isImm()) {
    int32_t Res = MO.getImm();
    // We don't check first 2 bits, because in these bits we could store first 2
    // bits of instruction address
    Res >>= 2;
    return Res;
  }

  assert((MO.isExpr()) && "Unexpected operand value!");

  Fixups.push_back(MCFixup::create(
      0, MO.getExpr(), MCFixupKind(Xtensa::fixup_xtensa_l32r_16), MI.getLoc()));
  return 0;
}

unsigned
XtensaMCCodeEmitter::getMemRegEncoding(const MCInst &MI, unsigned OpNo,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const {
  assert(MI.getOperand(OpNo + 1).isImm());

  long Res = static_cast<unsigned>(MI.getOperand(OpNo + 1).getImm());

  switch (MI.getOpcode()) {
  case Xtensa::S16I:
  case Xtensa::L16SI:
  case Xtensa::L16UI:
    if (Res & 0x1) {
      llvm_unreachable("Unexpected operand value!");
    }
    Res >>= 1;
    break;
  case Xtensa::S32I:
  case Xtensa::L32I:
  case Xtensa::S32I_N:
  case Xtensa::L32I_N:
  case Xtensa::S32F:
  case Xtensa::L32F:
  case Xtensa::S32C1I:
    if (Res & 0x3) {
      llvm_unreachable("Unexpected operand value!");
    }
    Res >>= 2;
    break;
  }

  unsigned OffBits = Res << 4;
  unsigned RegBits = getMachineOpValue(MI, MI.getOperand(OpNo), Fixups, STI);

  return ((OffBits & 0xFF0) | RegBits);
}

unsigned XtensaMCCodeEmitter::getImm8OpValue(const MCInst &MI, unsigned OpNo,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  int32_t Res = MO.getImm();

  assert(((Res >= -128) && (Res <= 127)) && "Unexpected operand value!");

  return (Res & 0xff);
}

unsigned
XtensaMCCodeEmitter::getImm8_sh8OpValue(const MCInst &MI, unsigned OpNo,
                                        SmallVectorImpl<MCFixup> &Fixups,
                                        const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  int32_t Res = MO.getImm();

  assert(((Res >= -32768) && (Res <= 32512) && ((Res & 0xff) == 0)) &&
         "Unexpected operand value!");

  return (Res & 0xffff);
}

unsigned
XtensaMCCodeEmitter::getImm12OpValue(const MCInst &MI, unsigned OpNo,
                                     SmallVectorImpl<MCFixup> &Fixups,
                                     const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  int32_t Res = MO.getImm();

  assert(((Res >= -2048) && (Res <= 2047)) && "Unexpected operand value!");

  return (Res & 0xfff);
}

unsigned
XtensaMCCodeEmitter::getUimm4OpValue(const MCInst &MI, unsigned OpNo,
                                     SmallVectorImpl<MCFixup> &Fixups,
                                     const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<long>(MO.getImm());

  assert(((Res >= 0) && (Res <= 15)) && "Unexpected operand value!");

  return Res & 0xf;
}

unsigned
XtensaMCCodeEmitter::getUimm5OpValue(const MCInst &MI, unsigned OpNo,
                                     SmallVectorImpl<MCFixup> &Fixups,
                                     const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<long>(MO.getImm());

  assert(((Res >= 0) && (Res <= 31)) && "Unexpected operand value!");

  return (Res & 0x1f);
}

unsigned
XtensaMCCodeEmitter::getShimm1_31OpValue(const MCInst &MI, unsigned OpNo,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<long>(MO.getImm());

  assert(((Res >= 1) && (Res <= 31)) && "Unexpected operand value!");

  return ((32 - Res) & 0x1f);
}

unsigned
XtensaMCCodeEmitter::getImm1_16OpValue(const MCInst &MI, unsigned OpNo,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<long>(MO.getImm());

  assert(((Res >= 1) && (Res <= 16)) && "Unexpected operand value!");

  return (Res - 1);
}

unsigned
XtensaMCCodeEmitter::getImm1n_15OpValue(const MCInst &MI, unsigned OpNo,
                                        SmallVectorImpl<MCFixup> &Fixups,
                                        const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<long>(MO.getImm());

  assert(((Res >= -1) && (Res <= 15) && (Res != 0)) &&
         "Unexpected operand value!");

  if (Res < 0)
    Res = 0;

  return Res;
}

unsigned
XtensaMCCodeEmitter::getImm32n_95OpValue(const MCInst &MI, unsigned OpNo,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<long>(MO.getImm());

  assert(((Res >= -32) && (Res <= 95)) && "Unexpected operand value!");

  return Res;
}

unsigned
XtensaMCCodeEmitter::getImm8n_7OpValue(const MCInst &MI, unsigned OpNo,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<long>(MO.getImm());

  assert(((Res >= -8) && (Res <= 7)) && "Unexpected operand value!");

  if (Res < 0)
    return Res + 16;

  return Res;
}

unsigned
XtensaMCCodeEmitter::getImm64n_4nOpValue(const MCInst &MI, unsigned OpNo,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<long>(MO.getImm());

  assert(((Res >= -64) && (Res <= -4) && ((Res & 0x3) == 0)) &&
         "Unexpected operand value!");

  return Res & 0x3f;
}

unsigned
XtensaMCCodeEmitter::getEntry_Imm12OpValue(const MCInst &MI, unsigned OpNo,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long res = static_cast<unsigned>(MO.getImm());

  assert(((res & 0x7) == 0) && "Unexpected operand value!");

  return res;
}

unsigned
XtensaMCCodeEmitter::getB4constOpValue(const MCInst &MI, unsigned OpNo,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  unsigned long Res = static_cast<unsigned>(MO.getImm());

  switch (Res) {
  case 0xffffffff:
    Res = 0;
    break;
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
    break;
  case 10:
    Res = 9;
    break;
  case 12:
    Res = 10;
    break;
  case 16:
    Res = 11;
    break;
  case 32:
    Res = 12;
    break;
  case 64:
    Res = 13;
    break;
  case 128:
    Res = 14;
    break;
  case 256:
    Res = 15;
    break;
  default:
    llvm_unreachable("Unexpected operand value!");
  }

  return Res;
}

unsigned
XtensaMCCodeEmitter::getB4constuOpValue(const MCInst &MI, unsigned OpNo,
                                        SmallVectorImpl<MCFixup> &Fixups,
                                        const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long Res = static_cast<unsigned>(MO.getImm());

  switch (Res) {
  case 32768:
    Res = 0;
    break;
  case 65536:
    Res = 1;
    break;
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
    break;
  case 10:
    Res = 9;
    break;
  case 12:
    Res = 10;
    break;
  case 16:
    Res = 11;
    break;
  case 32:
    Res = 12;
    break;
  case 64:
    Res = 13;
    break;
  case 128:
    Res = 14;
    break;
  case 256:
    Res = 15;
    break;
  default:
    llvm_unreachable("Unexpected operand value!");
  }

  return Res;
}

unsigned XtensaMCCodeEmitter::getShimmSeimm7_22OpValue(
    const MCInst &MI, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  long res = static_cast<unsigned>(MO.getImm());

  res -= 7;
  assert(((res & 0xf) == res) && "Unexpected operand value!");

  return res;
}

#include "XtensaGenMCCodeEmitter.inc"
