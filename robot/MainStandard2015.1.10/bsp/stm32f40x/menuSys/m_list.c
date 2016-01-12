
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "M_TypeDef.h"
#include "M_List.h"

#include "M_Sys.h"
#include "M_Test.h"
#include "M_Route.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*菜单*/
struct M_Type MainMenu[M_NUM];
struct M_Type M_System[M_NUM];
struct M_Type M_Test[M_NUM];
struct M_Type M_Route[M_NUM];
struct M_Type M_SD[M_NUM];
struct M_Type M_PID[M_NUM];
struct M_Type *CurrentMenu = MainMenu;//初始化当前菜单为主菜单

//主菜单下的子菜单
struct M_Type MainMenu[M_NUM]=
{   /*Name          菜单属性    	函数			节点*/
    {"1.System",	  M_SMenu,		    NULL,			{MainMenu, M_System} },
    {"2.Test",	  	M_SMenu,        NULL,     		{MainMenu, M_Test}   },
    {"3.Route",		  M_SMenu,        NULL,	    	{MainMenu, M_Route}  },
    {"4.PID",       M_SMenu,        NULL,			     {MainMenu, M_PID} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};
//用来放系统配置函数
struct M_Type M_System[M_NUM]=
{   /*Name			菜单属性		函数			节点(上下级指针)*/
    {"1.Gyro",      M_UFunc,        sys_Gyro,   	{MainMenu, NULL} },
    {"2.Encoder",   M_UFunc,        sys_Encoder,	{MainMenu, NULL} },
    {"3.GPS",       M_UFunc,        sys_GPS,    	{MainMenu, NULL} },
    {"4.Mecanum",   M_UFunc,        sys_Mecanum,	{MainMenu, NULL} },
    {"5.SA-ST",     M_UFunc,        sys_SA_ST,  	{MainMenu, NULL} },
    {"6.CAN",       M_UFunc,        sys_CAN,    	{MainMenu, NULL} },
    {"7.Handle",    M_UFunc,        sys_Handle, 	{MainMenu, NULL} },
		{"",       M_Empty,        NULL,			     {MainMenu, NULL} },
};
//用来放外设测试函数
struct  M_Type M_Test[M_NUM]=
{   /*Name          菜单属性		函数		 	节点(上下级指针)*/
    {"1.DataBack",  M_UFunc,        DataBackTest,	{MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};
//用来放路径相关函数
struct M_Type M_Route[M_NUM]=
{   /*Name          菜单属性		函数			节点(上下级指针)*/
    {"1.GoLine",    M_UFunc,        GoLineTest,		 {MainMenu, NULL} },
    {"2.GoCircle", 	M_UFunc,        GoCircleTest,  {MainMenu, NULL} },
    {"3.GoLean ",   M_UFunc,        GoLeanTest,		 {MainMenu, NULL} },
    {"4.STAR ",     M_UFunc,        STAR,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};


struct M_Type M_PID[M_NUM]=
{   /*Name          菜单属性		函数			节点(上下级指针)*/
    {"1.pid_A ",    M_UFunc,        pid_set_A,		 {MainMenu, NULL} },
    {"2.pid_str ",  M_UFunc,        GoLineTest,		 {MainMenu, NULL} },
    {"3.pid_cir ",  M_UFunc,        GoCircleTest,	 {MainMenu, NULL} },
		{"4.pid_lean ", M_UFunc,        GoLeanTest,		 {MainMenu, NULL} },
		{"5.keep",      M_UFunc,        KeepTest,			 {MainMenu, NULL} },
		{"6.RouteTest", M_UFunc,        RouteTest,			{MainMenu, NULL} },
		{"7.test_elevator",    M_UFunc,        test_elevator,			     {MainMenu, NULL} },
		{"8.stepper_fan",    M_UFunc,        stepper_fan,			     {MainMenu, NULL} },
};

struct M_Type M_SD[M_NUM]=   
{   /*Name          菜单属性		函数			节点(上下级指针)*/
    {"1.ReInit",    M_UFunc,        SD_ReInit,		{MainMenu, NULL} },
    {"2.WalkGRD",   M_UFunc,        SD_Walkground,	{MainMenu, NULL} },
    {" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
    {" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
    {" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
    {" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
    {" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
		{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/********************************    END OF FILE    ***************************/
