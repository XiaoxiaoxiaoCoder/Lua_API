
/*
 * gcc test.c -llua -lm -ldl
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

int main(int argc, char *argv[])
{
    lua_State *L = lua_open();
    luaL_openlibs(L);
    
    lua_pushboolean(L, 1);
    lua_pushnumber(L, 10);
    lua_pushnil(L);
    lua_pushstring(L, "Hello");
    lua_pushlstring(L, "World!!!", 6);

    stackDump(L);

    lua_pushvalue(L, -4);
    stackDump(L);

    lua_remove(L, 3);
    stackDump(L);

    lua_insert(L, -2);
    stackDump(L);

    lua_replace(L, 1);
    stackDump(L);


    lua_close(L);
    
    return 0;
}
