.section .data

buffer:
    .space 256          // allocating 256 byte buffer

// messages
dir_msg:
    .asciz "Current Directory: "
newline:
    .asciz "\n"
file_msg:
    .asciz "Creating and writing to test file...\n"
read_msg:
    .asciz "Read from file: "
chmod_msg:
    .asciz "Changed file permissions\n"

test_file:
    .asciz "test.txt"
file_content:
    .asciz "Hello from AArch64 assembly!\n"

.section .text
.global _start

_start:
    mov x0, #1          // stdout
    ldr x1, =dir_msg    // message address
    mov x2, #19         // message length
    mov x8, #64         // write syscall
    svc #0

    // getting cwd
    ldr x0, =buffer
    mov x1, #256
    mov x8, #17         // getcwd syscall
    svc #0
    mov x10, x0         // saving length returned by getcwd

    // Printing the cwd
    mov x0, #1          // stdout
    ldr x1, =buffer
    mov x2, x10         // length of the cwd string
    mov x8, #64         // write syscall
    svc #0

    // newline
    mov x0, #1
    ldr x1, =newline
    mov x2, #1
    mov x8, #64
    svc #0

    // print file ops message
    mov x0, #1
    ldr x1, =file_msg
    mov x2, #35
    mov x8, #64
    svc #0

    // create and open file
    mov x0, #-100       // AT_FDCWD (current directory)
    ldr x1, =test_file  // filename
    mov x2, #65         // O_WRONLY | O_CREAT
    mov x3, #0644       // file permissions (rw-r--r--)
    mov x8, #56         // syscall: openat
    svc #0
    mov x9, x0          // saving file descriptor

    // writing to the file
    mov x0, x9
    ldr x1, =file_content // content
    mov x2, #28         // content length
    mov x8, #64         // write syscall
    svc #0

    // close the file
    mov x0, x9
    mov x8, #57         // syscall: close
    svc #0

    // changing file permissions
    mov x1, #0755       // nwew permissions rwxr-xr-x
    ldr x0, =test_file  // filename
    mov x8, #52         // syscall: chmod
    svc #0

    // print chmod message
    mov x0, #1
    ldr x1, =chmod_msg
    mov x2, #24
    mov x8, #64
    svc #0

    // open file
    mov x0, #-100       // AT_FDCWD
    ldr x1, =test_file  // filename
    mov x2, #0          // O_RDONLY
    mov x8, #56         // syscall: openat
    svc #0
    mov x9, x0

    // read from the file
    mov x0, x9
    ldr x1, =buffer     // buffer for reading
    mov x2, #256        // buffer size
    mov x8, #63         // syscall: read
    svc #0
    mov x10, x0         // saving number of bytes read

    // close the file
    mov x0, x9
    mov x8, #57         // syscall: close
    svc #0

    // print read message
    mov x0, #1
    ldr x1, =read_msg
    mov x2, #16
    mov x8, #64
    svc #0

    // Print file contents
    mov x0, #1
    ldr x1, =buffer
    mov x2, x10         // number of bytes read
    mov x8, #64
    svc #0

    // Exit with code 1
    mov x0, #1          // Return code 1
    mov x8, #93         // syscall: exit
    svc #0