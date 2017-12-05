
// get member's addr offset in its struct
#define offsetof(TYPE, MEMBER) 				((size_t) &((TYPE *)0)->MEMBER)

// get member's struct pointer
#define container_of(ptr, type, member) 	({          \
    const typeof(((type *)0)->member)*__mptr = (ptr);    \  
    (type *)((char *)__mptr - offsetof(type, member));})


#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

