#ifndef CONSTANT_POOL_H_
#define CONSTANT_POOL_H_

class ConstantPool {
public:
	ConstantPool();
	virtual ~ConstantPool();

	void Write(std::ostream& stream);
};

#endif /* CONSTANT_POOL_H_ */
