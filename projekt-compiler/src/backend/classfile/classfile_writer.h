#ifndef CLASSFILE_WRITER_H_
#define CLASSFILE_WRITER_H_

#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <frontend/Graphs.h>
#include <backend/codegen/Bytecode.h>
#include "constant_pool.h"

/**
 * Klasse zum Schreiben einer einfachen .class-Datei (Nur Methoden, keine
 * Fields, keine Vererbung) auf einen Stream.
 */
class ClassfileWriter {
 public:
  /**
   * Die Versionsnummer der zu schreibenden .class-Datei.
   */
  enum ClassfileVersion {
    JAVA_7
  };

  /**
   * Erstellt einen neuen Writer zum Schreiben einer .class-Datei der
   * Version 'version' mit dem ConstantPool 'constantPool' auf den
   * Stream 'out'. Der Bytecode wird in der map 'codeFunctions' gehalten, Map
   * Funktionsname -> Bytecode.
   */
  ClassfileWriter(ClassfileVersion version, ConstantPool* constantPool,
                    Graphs& graphs,
                    const std::map<std::string, codegen::Bytecode&> codeFunctions,
                    std::ostream* out);

  virtual ~ClassfileWriter();

  /**
   * Schreibt die durch diesen Writer festgelegte .class-Datei auf den
   * Ausgabestream.
   */
  void WriteClassfile();

 private:


  /**
   * Graph aus dem Frontend
   */
  Graphs graphs_;

  /**
   * Bytecode writer
   */
  Bytecode_writer writer;

  /**
   * Die Magic Number aller .class-Dateien.
   */
  static const char kMagicNumber[];

  /**
   * Eine Map die Java-Versionsnummern auf den entsprechenden
   * Classfile-Versionsnummer-Eintrag (4 byte) mappt.
   */
  static std::map<ClassfileVersion, std::array<char, 4>> kVersionNumbers;

  /**
   * Der Ausgabestream auf den wir die Datei schreiben.
   */
  std::ostream *out_;

  /**
   * Die Versionsnummer der zu schreibenden .class-Datei.
   */
  ClassfileVersion version_;

  /**
   * Der ConstantPool der zu schreibenden Class-Datei.
   */
  std::shared_ptr<ConstantPool> constant_pool_;

  /**
   * Mappt Funktionsnamen auf ihren zugehörigen Bytecode.
   */
  const std::map<std::string, codegen::Bytecode&> code_functions_;

  /**
   * Konstante zum Schreiben für nicht verwendete Elemente in der Class-Datei.
   * Vorerst für Interface und Field - wird daher auf den Wert 0x0000 gesetzt.
   */
  static const char kNotRequired[];

  /**
   * Konstante zum Schreiben für öffentliches Konstrukt.
   */
  static const char kPublicAccessFlag[];

  /**
   * Konstante zum Schreiben für public + static access flag
   */
  static const char kPublicStaticAccessFlag[];

  /**
   * Konstante: Max Stacktiefe lokaler Stack. Nicht viel da wir globalen Op
   * Stack nutzen.
   */
  static const uint16_t kMaxStack;

  /**
   * Schreibt 0xCAFEBABE.
   */
  void WriteMagicNumber();

  /**
   * Schreibt die zu 'version_' gehörende Versionsnr.
   */
  void WriteVersionNumber();

  /**
   * Schreibt den Constant Pool.
   */
  void WriteConstantPool();

  /**
   * Schreibt die Access-Flags. Immer 0xXXXX. TODO
   */
  void WriteAccessFlags();

  /**
   * Schreibt den Classname. Immer 0xXXXX. TODO
   */
  void WriteClassName();

  /**
   * Schreibt den Super-Class name. Immer (Pointer auf java/lang/Object)
   * 0xXXXX. TODO
   */
  void WriteSuperClassName();

  /**
   * Schreibt die von uns implementierten Interfaces. Immer 0x0000.
   */
  void WriteInterfaces();

  /**
   * Schreibt die Fields der Klasse. Immer 0x0000.
   */
  void WriteFields();

  /**
   * Schreibt die Methoden.
   */
  void WriteMethods();

  /**
   * Schreibt die <init> Methode.
   */
  void WriteInitMethod();

  /**
   * Schreibt die <clinit> Methode.
   */
  void WriteClInitMethod();

  /**
   * Schreibt die Attribute, insb. den Methoden-Bytecode.
   */
  void WriteAttributes(const std::string &key);
};
#endif /* CLASSFILE_WRITER_H_ */
