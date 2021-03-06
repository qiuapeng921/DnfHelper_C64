#include "pch.h"

int 任务名称()
{
	wstring 局_名称;
	ULONG64 局_首地址 = 读写_读长整数(读写_读长整数(任务基址) + 任务首地址);
	ULONG64 局_尾地址 = 读写_读长整数(读写_读长整数(任务基址) + 任务尾地址);
	size_t 局_任务数量 = (局_尾地址 - 局_首地址) / 8;
	for (size_t 局_完成次数 = 0; 局_完成次数 < 局_任务数量; 局_完成次数++)
	{
		ULONG64 局_任务地址 = 读写_读长整数(局_首地址 + 局_完成次数 * 8);
		ULONG 局_任务类型 = 读写_读整数型(局_任务地址 + 任务类型);
		if (局_任务类型 == 0)
		{
			ULONG64 任务编号 = 读写_读长整数(局_任务地址);
			ULONG 局_长度 = 读写_读整数型(局_任务地址 + 40);
			if (局_长度 > 7)
			{
				局_名称 = Unicode转Ansi(读写_读字节集(读写_读整数型(局_任务地址 + 16), 100));
			}
			else
			{
				局_名称 = Unicode转Ansi(读写_读字节集(局_任务地址 + 16, 100));
			}
			系统公告(L"当前任务" + 局_名称);
			return 任务编号;
		}
	}
	return 0;
}


int 获取隐藏任务()
{
	ULONG64 局部指针 = 读写_读长整数(任务基址);
	ULONG64 局部首址 = 读写_读长整数(局部指针 + 16);
	ULONG64 局部尾址 = 读写_读长整数(局部指针 + 24);
	size_t 对象数量 = (局部尾址 - 局部首址) / 16;
	for (size_t 局部计次 = 0; 局部计次 < 对象数量; 局部计次++)
	{
		ULONG64 对象地址 = 读写_读长整数(局部首址 + 局部计次 * 16);
		ULONG 对象类型 = 读写_读整数型(对象地址 + 任务类型);
		if (对象类型 == 11)
		{
			return 读写_读长整数(对象地址);
		}
	}
	return 0;
}

int 获取主线任务()
{
	ULONG64 局部指针 = 读写_读长整数(任务基址);
	ULONG64 局部首址 = 读写_读长整数(局部指针 + 任务首地址);
	ULONG64 局部尾址 = 读写_读长整数(局部指针 + 任务尾地址);
	size_t 对象数量 = (局部尾址 - 局部首址) / 8;
	for (size_t 局部计次 = 0; 局部计次 < 对象数量; 局部计次++)
	{
		ULONG64 对象地址 = 读写_读长整数(局部首址 + 局部计次 * 8);
		ULONG 对象类型 = 读写_读整数型(对象地址 + 任务类型);
		if (对象类型 == 0)
		{
			return 读写_读长整数(对象地址);
		}
	}
	return 0;
}


int 完成次数(int 解密次数)
{
	if (解密次数 < 512) return 解密次数;
	if (解密次数 == 512) return 1;
	vector<int>  局_次数数组;
	局_次数数组.resize(3);
	int sub = 0;
	局_次数数组[0] = 解密次数 % 512;
	sub = 解密次数 - 局_次数数组[0];
	局_次数数组[1] = sub < 262144 ? (sub / 512) : ((sub % 262144) / 512);
	sub = sub - 局_次数数组[0] * 512;
	局_次数数组[2] = sub < 262144 ? 0 : (sub / 262144);
	int max = *max_element(局_次数数组.begin(), 局_次数数组.end());
	if (max == 0) max = 1;
	return max;
}

int 任务次数(int id)
{
	ULONG64 局_首址 = 读写_读长整数(读写_读长整数(任务基址) + 16);
	ULONG64 局_尾址 = 读写_读长整数(读写_读长整数(任务基址) + 24);
	size_t 局_数量 = (局_尾址 - 局_首址) / 16;
	for (size_t 局_计次 = 0; 局_计次 < 局_数量; 局_计次++)
	{
		ULONG64 对象地址 = 读写_读长整数(局_首址 + 局_计次 * 16);
		ULONG64 任务编号 = 读写_读长整数(对象地址);
		if (任务编号 == id)
		{
			return 超级解密(局_首址 + 局_计次 * 16 + 8);
		}
	}
	return -1;
}

bool 获取城镇完成(wstring 条件)
{
	if (条件.empty()) return false;
	wstring 分割文本(L"[hunt enemy][meet npc][hunt monster][seek n meet npc][quest clear][reach the range][look cinematic][question]");
	return 分割文本.find(条件.c_str()) != 分割文本.npos;
}

bool 获取进图完成(wstring 条件)
{
	if (条件.empty()) return false;
	wstring 分割文本(L"[condition under clear][clear map][seeking]");
	return 分割文本.find(条件.c_str()) != 分割文本.npos;
}

int 获取任务地图(int 任务编号)
{
	ULONG64	局_地址 = 读写_读长整数(任务基址);
	ULONG64	局_首址 = 读写_读长整数(局_地址 + 16);
	ULONG64	局_尾址 = 读写_读长整数(局_地址 + 24);
	size_t 局_数量 = (局_尾址 - 局_首址) / 8;
	for (size_t 局_计次 = 0; 局_计次 < 局_数量; 局_计次++)
	{
		ULONG64 局_指针 = 读写_读长整数(局_首址 + 局_计次 * 8);
		if (读写_读长整数(局_指针) == 任务编号)
		{
			ULONG 局_数据 = 读写_读整数型(局_指针 + 任务_地图);
			return 读写_读长整数(局_数据);
		}
	}
	return 0;
}

bool 是否有主线任务()
{

	ULONG64 对象首部 = 读写_读长整数(读写_读长整数(任务基址) + 任务首地址);
	ULONG64 对象尾部 = 读写_读长整数(读写_读长整数(任务基址) + 任务尾地址);
	size_t 对象个数 = (对象尾部 - 对象首部) / 8;
	for (size_t 对象计次 = 0; 对象计次 < 对象个数; 对象计次++)
	{
		ULONG64 对象地址 = 读写_读长整数(对象首部 + 对象计次 * 8);
		ULONG 对象类型 = 读写_读整数型(对象地址 + 任务类型);
		if (对象类型 == 0)
		{
			return true;
		}
	}
	return false;
}


int 获取处理方式(int 对象编号)
{
	wstring 文本条件;
	ULONG64	局部地址 = 读写_读长整数(任务基址);
	ULONG64	局_首址 = 读写_读长整数(局部地址 + 16);
	ULONG64	局_尾址 = 读写_读长整数(局部地址 + 24);
	size_t 对象数量 = (局_尾址 - 局_首址) / 8;
	for (size_t 局部计次 = 0; 局部计次 < 对象数量; 局部计次++)
	{
		ULONG64 局部指针 = 读写_读长整数(局_首址 + 局部计次 * 8);
		if (读写_读长整数(局部指针) == 对象编号)
		{
			文本条件 = Unicode转Ansi(读写_读字节集(读写_读整数型(局部指针 + 任务条件), 100));
			系统公告(L"任务对应条件 - " + 文本条件);
			if (文本条件.find(L"[quest clear]") != 文本条件.npos)
			{
				return 1;
			}
			if (文本条件.find(L"[seeking]") != 文本条件.npos)
			{
				if (获取任务地图(对象编号) > 0)
				{
					return 2;
				}
				else
				{
					return 3;
				}
			}
			if (获取城镇完成(文本条件))
			{
				return 1;
			}
			if (获取进图完成(文本条件))
			{
				return 2;
			}
		}
	}
	return -1;
}

bool 获取处理结果(int 对象编号)
{
	int 完成状态, 处理方式;
	完成状态 = 完成次数(任务次数(对象编号));
	处理方式 = 获取处理方式(对象编号);
	if (完成状态 == -1)
	{
		接受Call(对象编号);
	}
	if (完成状态 == 0)
	{
		提交Call(对象编号);
	}
	if (处理方式 == 1)
	{
		完成Call(对象编号, 完成状态);
	}
	if (处理方式 == 2)
	{
		全局_自动.全局任务 = 对象编号;
		全局_自动.副本编号 = 获取任务地图(对象编号);
		return true;
	}
	if (处理方式 == 3)
	{
		系统公告(L"当前为材料任务,请购买材料");
	}
	return false;
}