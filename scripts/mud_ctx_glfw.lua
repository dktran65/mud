-- mud library
-- mud ctx glfw module

function mud_ctx_glfw()
    includedirs {
        path.join(MUD_3RDPARTY_DIR, "glfw", "include"),
    }
end

function uses_mud_ctx_glfw()
    configuration { "not asmjs" }
        links {
            "glfw",
        }
    
    configuration { "asmjs" }
        linkoptions {
            "-s USE_GLFW=3",
        }
        
    configuration {}
end

mud.ctx.glfw = mud_module("mud", "ctx-glfw", MUD_SRC_DIR, "ctx-glfw", mud_ctx_glfw, uses_mud_ctx_glfw, false, { glfw, mud.math, mud.ctx })

function mud_ctx_backend()
    return mud.ctx.glfw 
end
