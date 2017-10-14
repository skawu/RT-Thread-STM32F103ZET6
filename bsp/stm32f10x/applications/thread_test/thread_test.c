/***********************************************************************************
 * 文 件 名   : thread_test.c
 * 负 责 人   : skawu
 * 创建日期   : 2017年10月13日
 * 文件描述   : RTT线程创建与初始化使用测试文件
 * 版权说明   : Copyright (c) 2016-2017   深圳方糖电子有限公司
 * 其    他   :
 * 修改日志   :
***********************************************************************************/

#include <stm32f10x.h>
#include "thread_test.h"
#include <rtthread.h>
#include <rthw.h>

/*
    一、动态线程

    初始化两个动态线程，它们拥有相同的入口函数，相同的优先级
    但是它们的入口参数不同

    二、任务间同步与通信

    关闭中断进行全局变量的访问，关闭中断会导致整个系统不能响应外部中断，
    所以需保证关闭中断的时间非常短。
*/
#define THREAD_PRIORITY     25
#define THREAD_STACK_SIZE   512
#define THREAD_TIMESLICE    5

/* 定义静态全局变量 */
static rt_uint32_t count = 1;

/* 指向线程控制块的指针 */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

/* 静态全局变量操作函数 */
//待添加

/* 线程入口 */
//rt_uint32_t count = 0;		//串口看上去是同时打印tid1和tid2的数值后延时1s，而不是tid1延时1s，tid2延时1s
static void thread_entry(void *parameter)
{
	rt_base_t level;
	rt_uint32_t no = (rt_uint32_t) parameter;   //获取线程的入口参数

	while (1)
	{
		//关闭中断
		level = rt_hw_interrupt_disable();
		count += no;
		//恢复中断
		rt_hw_interrupt_enable(level);
		//打印线程计数值输出
		rt_kprintf("thread%d count: %d\n", no, count);
		//休眠100个OS Tick
		rt_thread_delay(100);
	}
}

/* 用户层调用创建线程 */
int app_init_thread_test(void)
{
	//创建线程1
	tid1 = rt_thread_create("thread1", thread_entry, (void *)1, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

	if (tid1 != RT_NULL)
	{
		rt_thread_startup(tid1);
	}
	else
	{
		return -1;
	}

	//创建线程2
	tid2 = rt_thread_create("thread2", thread_entry, (void *)2, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

	if (tid2 != RT_NULL)
	{
		rt_thread_startup(tid2);
	}
	else
	{
		return -1;
	}

	return 0;
}