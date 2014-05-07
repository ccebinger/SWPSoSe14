#ifndef CONSTANT_POOL_H_
#define CONSTANT_POOL_H_

#include <iostream>

/**
 * Repräsentiert einen Constant Pool einer .class-Datei, Version 7.
 * Kann auf einem std::ostream ausgegeben werden, als Teil einer .class-Datei.
 */
class ConstantPool {
public:
	ConstantPool();
	virtual ~ConstantPool();

	/**
	 * Schreibt den Pool im .class-Dateiformat (Version 7) in den stream.
	 */
	void Write(std::ostream& stream);
};

#endif /* CONSTANT_POOL_H_ */
