#ifndef CLASSFILE_WRITER_H_
#define CLASSFILE_WRITER_H_

#include <iostream>

class ClassfileWriter {
public:
	enum ClassfileVersion {
		JAVA_7
	};

	ClassfileWriter(ClassfileVersion version, std::ostream& out);
	virtual ~ClassfileWriter();

	void WriteClassfile();

private:
	std::ostream& out_;
	ClassfileVersion version_;

	void WriteMagicNumber();
	void WriteVersionNumber();
	void WriteConstantPool();
	void WriteAccessFlags();
	void WriteClassName();
	void WriteSuperClassName();
	void WriteInterfaces();
	void WriteFields();
	void WriteMethods();
	void WriteAttributes();
};

#endif /* CLASSFILE_WRITER_H_ */
