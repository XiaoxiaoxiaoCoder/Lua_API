
/*
 * gcc test.c -llua -lm -ldl
 */
 
 /**
  *	int lua_istable (lua_State *L, int index);
  *	当给定索引的值是一张表时，返回 1 ，否则返回 0 。
  */
  
 /**
  *	int lua_getfield (lua_State *L, int index, const char *k);
  * 把 t[k] 的值压栈， 这里的 t 是索引指向的值。 在 Lua 中，这个函数可能触发对应 "index" 事件对应的元方法
  *
  */
  
 /**
  *	int lua_isfunction (lua_State *L, int index);
  * 当给定索引的值是一个函数（ C 或 Lua 函数均可）时，返回 1 ，否则返回 0 。
  *
  */

 /**
  *	const char *lua_tostring (lua_State *L, int index);
  * 把给定索引处的 Lua 值转换为一个 C 字符串
  *
  */
  
 /**
  *	void lua_setfield (lua_State *L, int index, const char *k);
  * 做一个等价于 t[k] = v 的操作， 这里 t 是给出的索引处的值， 而 v 是栈顶的那个值
  *
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
		
	lua_getglobal(L, "color");
	stackDump(L);
	
	if(!lua_istable(L, -1))
	{
		printf("It's not table\n");
		lua_close(L);
		return 0;
	}
	
	//读取table中字段的值,将值压入栈,因此下一个获取值时,table的index已经改变了
	lua_getfield(L, -1, "r");
	lua_getfield(L, -2, "g");
	lua_getfield(L, -3, "b");
	stackDump(L);
	
	if(!lua_isnumber(L, -3))
	{
		printf("r is not number\n");
		lua_close(L);
		return 0;
	}
	
	if(!lua_isnumber(L, -2))
	{
		printf("g is not number\n");
		lua_close(L);
		return 0;
	}
	
	if(!lua_isnumber(L, -1))
	{
		printf("b is not number\n");
		lua_close(L);
		return 0;
	}
	
	
	int r = lua_tointeger(L, -3);
	int g = lua_tointeger(L, -2);
	int b = lua_tointeger(L, -1);

	printf("r[%d] g[%d] b[%d]\n", r, g, b);
	
	//调用函数
	lua_getglobal(L, "add");
	if(!lua_isfunction(L, -1))
	{
		printf("It's not function\n");
		lua_close(L);
		return 0;
	}
	
	//压入参数
	lua_pushnumber(L, 10);	//第一个参数
	lua_pushnumber(L, 20);	//第二个参数
	
	iRet = lua_pcall(L, 2, 1, 0);
	if(iRet)
	{
		printf("%s\n", lua_tostring(L, -1));
		lua_close(L);
		return 0;
	}
	
	if(!lua_isnumber(L, -1))
	{
		printf("Result is not number\n");
		lua_close(L);
		return 0;
	}
	iRet = lua_tonumber(L, -1);
	printf("Ret[%d]\n", iRet);
	
    lua_close(L);
    
    return 0;
}
