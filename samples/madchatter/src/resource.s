.global _binary_resource_res
.global _binary_resource_res_size

# The following directive works for most assemblers
.section .rodata

# On OSX, if the directive above fails, comment it.
# and uncomment the following line
#.section __DATA, __const

#
.balign 4
_binary_resource_res:
    .incbin "resource.res"
1:
_binary_resource_res_size:
.int 1b - _binary_resource_res

