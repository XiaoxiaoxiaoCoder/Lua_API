
/*
 * gcc test.c -llua -lm
 */


#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    lua_State *L = lua_open();
    printf("begininit : stack top = %d\n", lua_gettop(L));
    
    lua_pushnumber(L, 5);
    lua_pushnumber(L, 4);
    printf("pushargvs : stack top = %d\n", lua_gettop(L));
    
    lua_settop(L, 1);
    printf("settopfun : stack top = %d\n", lua_gettop(L));
    lua_settop(L, 0);
    printf("settopfun : stack top = %d\n", lua_gettop(L));

    lua_close(L);
    
    return 0;
}
