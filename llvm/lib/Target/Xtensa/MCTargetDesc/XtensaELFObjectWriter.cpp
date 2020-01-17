//===-- XtensaMCObjectWriter.cpp - Xtensa ELF writer ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/XtensaMCTargetDesc.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

namespace {
class XtensaObjectWriter : public MCELFObjectTargetWriter {
public:
  XtensaObjectWriter(uint8_t OSABI);

  virtual ~XtensaObjectWriter();

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCSymbol &Sym,
                               unsigned Type) const override;
};
} // namespace

XtensaObjectWriter::XtensaObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(false, OSABI, ELF::EM_XTENSA,
                              /*HasRelocationAddend=*/true) {}

XtensaObjectWriter::~XtensaObjectWriter() {}

unsigned XtensaObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                                          const MCFixup &Fixup,
                                          bool IsPCRel) const {
  MCSymbolRefExpr::VariantKind Modifier = Target.getAccessVariant();

  switch ((unsigned)Fixup.getKind()) {
  case FK_Data_4:
    if (Modifier == MCSymbolRefExpr::VariantKind::VK_TPOFF)
      return ELF::R_XTENSA_TLS_TPOFF;
    else
      return ELF::R_XTENSA_32;
  default:
    return ELF::R_XTENSA_SLOT0_OP;
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createXtensaObjectWriter(uint8_t OSABI, bool IsLittleEndian) {
  return llvm::make_unique<XtensaObjectWriter>(OSABI);
}

bool XtensaObjectWriter::needsRelocateWithSymbol(const MCSymbol &Sym,
                                                 unsigned Type) const {
  return false;
}
