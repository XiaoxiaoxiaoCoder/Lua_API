
/*
 * gcc test.c -llua -lm -ldl
 */
 
 /**
  *	int luaL_loadfile (lua_State *L, const char *filename);
  * 把一个文件加载为 Lua 代码块,代码块的名字被命名为 filename。
  * 如果 filename 为NULL,它从标准输入加载。如果文件第一行以#开头,则忽略这一行
  *	LUA_OK: 没有错误；
  *	LUA_ERRSYNTAX: 在预编译时碰到语法错误；
  *	LUA_ERRMEM: 内存分配错误；
  *	LUA_ERRGCMM: 在运行 __gc 元方法时出错了。 （这个错误和代码块加载过程无关，它是由垃圾收集器引发的。）
  *
  */
  
 /**
  *	int lua_pcall (lua_State *L, int nargs, int nresults, int msgh);
  * 以保护模式调用一个函数。
  * nargs 为压入栈的参数个数, 函数被调用完后所有参数都会出栈。而函数返回值此时入栈,返回值个数调整为nresults
  *	如果 msgh 是 0 ， 返回在栈顶的错误消息就和原始错误消息完全一致。 否则， msgh 就被当成是 错误处理函数 在栈上的索引位置
  *	LUA_OK (0): 成功。
  *	LUA_ERRRUN: 运行时错误。
  *	LUA_ERRMEM: 内存分配错误。对于这种错，Lua 不会调用错误处理函数。
  *	LUA_ERRERR: 在运行错误处理函数时发生的错误。
  *	LUA_ERRGCMM: 在运行 __gc 元方法时发生的错误。 （这个错误和被调用的函数无关。）
  *
  */
  
 /**
  *	int lua_getglobal (lua_State *L, const char *name);
  * 把全局变量 name 里的值压栈，返回该值的类型。
  *
  */

 /**
  *	int lua_isnumber (lua_State *L, int index);
  * 当给定索引的值是一个数字，或是一个可转换为数字的字符串时，返回 1 ，否则返回 0 。
  *
  */
 
 /**
  *	lua_Integer lua_tointeger (lua_State *L, int index);
  * 将给定索引处的 Lua 值转换为带符号的整数类型 lua_Integer。 这个 Lua 值必须是一个整数，或是一个可以被转换为整数的数字或字符串； 否则，lua_tointegerx 返回 0 。
  *	
  * 类似的还有
  * lua_stringtonumber
  * lua_toboolean
  * lua_tolstring
  * lua_tonumber
  * lua_tostring
  */
  
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>


/*
 * 从栈底到栈顶依次遍历整个堆栈
 */
static void stackDump(lua_State* L)
{
    int i;
    int top = lua_gettop(L);
    for(i = 1; i <= top; ++i)
    {
        int t = lua_type(L, i);
        switch(t)
        {
        case LUA_TSTRING:
            printf("'%s'", lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            printf(lua_toboolean(L, i) ? "true": "false");
            break;
        case LUA_TNUMBER:
            printf("'%g'", lua_tonumber(L, i));
            break;
        default:
            printf("'%s'", lua_typename(L, t));
            break;
        }
        printf("   ");

    }
    printf("\n");
}

void PrintfLuaType(lua_State* L, int type)
{
	printf("'%s'", lua_typename(L, type));
}

int main(int argc, char *argv[])
{
    lua_State *L = lua_open();
    luaL_openlibs(L);
    
	//加载配置文件
    int iRet = luaL_loadfile(L, "config.lua");
	if(iRet)
	{
		printf("laodfile config.lua fail!!\n");
		lua_close(L);
		return 0;
	}
	
	iRet = lua_pcall(L, 0, 0, 0);
	if(iRet)
	{
		printf("lua_pcall fail!!\n");
		lua_close(L);
		return 0;
	}
		
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	stackDump(L);
	
	//width
	if(!lua_isnumber(L, -2))
	{
		printf("width is not number\n");
		lua_close(L);
		return 0;
	}
	
	//height
	if(!lua_isnumber(L, -1))
	{
		printf("height is not number\n");
		lua_close(L);
		return 0;
	}
	int iHeight = lua_tointeger(L, -2);
	int iWidth = lua_tointeger(L, -1);

	printf("Width[%d] Height[%d]\n", iWidth, iHeight);
	
    lua_close(L);
    
    return 0;
}
