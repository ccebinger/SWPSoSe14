#ifndef CLASSFILE_WRITER_H_
#define CLASSFILE_WRITER_H_

#include <iostream>

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
	 * Stream 'out'.
	 */
	ClassfileWriter(ClassfileVersion version, const ConstantPool& constantPool,
			std::ostream& out);

	virtual ~ClassfileWriter();

	/**
	 * Schreibt die durch diesen Writer festgelegte .class-Datei auf den
	 * Ausgabestream.
	 */
	void WriteClassfile();

private:
	/**
	 * Der Ausgabestream auf den wir die Datei schreiben.
	 */
	std::ostream& out_;

	/**
	 * Die Versionsnummer der zu schreibenden .class-Datei.
	 */
	ClassfileVersion version_;

	/**
	 * Der ConstantPool der zu schreibenden Class-Datei.
	 */
	const ConstantPool& constant_pool_;

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
	 * Schreibt die Attribute, insb. den Methoden-Bytecode.
	 */
	void WriteAttributes();
};

#endif /* CLASSFILE_WRITER_H_ */
