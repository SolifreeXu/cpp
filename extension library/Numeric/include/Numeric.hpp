#pragma once

// ��������
enum class IntegerType
{
	ALL,      // ����
	NATURAL,  // ��Ȼ��
	POSITIVE, // ����
	NEGATIVE, // ����
};

// ���Ϸ���
bool valid(const char* _string, IntegerType _type);
