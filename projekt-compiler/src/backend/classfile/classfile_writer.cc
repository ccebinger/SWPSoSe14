#include "classfile_writer.h"

ClassfileWriter::ClassfileWriter(ClassfileVersion version,
		const ConstantPool& constantPool, std::ostream& out) :
		out_(out), version_(version), constant_pool_(constantPool) {
}

ClassfileWriter::~ClassfileWriter() {
}

void ClassfileWriter::WriteClassfile() {
	// TODO
}

void ClassfileWriter::WriteMagicNumber() {
	// TODO
}

void ClassfileWriter::WriteVersionNumber() {
	// TODO
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
