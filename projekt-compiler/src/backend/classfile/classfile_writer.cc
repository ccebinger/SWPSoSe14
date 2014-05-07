#include "classfile_writer.h"

#include <array>
#include <iostream>
#include <map>

const char ClassfileWriter::kMagicNumber[] { '\xCA', '\xFE', '\xBA', '\xBE' };

std::map<ClassfileWriter::ClassfileVersion, const std::array<const char, 4>> ClassfileWriter::kVersionNumbers {
		{ ClassfileWriter::ClassfileVersion::JAVA_7, { '\x00', '\x00', '\x00',
				'\x33' } } };

ClassfileWriter::ClassfileWriter(ClassfileVersion version,
		const ConstantPool& constantPool, std::ostream& out) :
		out_(out), version_(version), constant_pool_(constantPool) {
}

ClassfileWriter::~ClassfileWriter() {
}

void ClassfileWriter::WriteClassfile() {
	WriteMagicNumber();
	WriteVersionNumber();
	WriteConstantPool();
	WriteAccessFlags();
	WriteClassName();
	WriteSuperClassName();
	WriteInterfaces();
	WriteFields();
	WriteMethods();
	WriteAttributes();
}

void ClassfileWriter::WriteMagicNumber() {
	out_.write(kMagicNumber, sizeof(kMagicNumber));
}

void ClassfileWriter::WriteVersionNumber() {
	out_.write(kVersionNumbers[version_].data(),
			sizeof(kVersionNumbers[version_].data()));
}

void ClassfileWriter::WriteConstantPool() {
	// TODO
}

void ClassfileWriter::WriteAccessFlags() {
	// TODO
}

void ClassfileWriter::WriteClassName() {
	// TODO
}

void ClassfileWriter::WriteSuperClassName() {
	// TODO
}

void ClassfileWriter::WriteInterfaces() {
	// TODO
}

void ClassfileWriter::WriteFields() {
	// TODO
}

void ClassfileWriter::WriteMethods() {
	// TODO
}

void ClassfileWriter::WriteAttributes() {
	// TODO
}
