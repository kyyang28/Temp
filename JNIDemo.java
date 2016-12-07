
public class JNIDemo {

    /* Only executed once before instantiated an object */
    static {
        /* 1. Load library */
        System.loadLibrary("native");   // generate libnative.so for linux
    }

    public native int [] hello(int [] arr);
    //public native static void hello();
    
    public static void main(String args[])
    {
        JNIDemo d = new JNIDemo();

        int [] a = {1, 2, 3}; 
        int [] b = null;
        
        /* 2. Mapping java method hello() to c function c_hello() */
        
        
        /* 3. Call */
        b = d.hello(a);

        for (int i = 0; i < b.length; i++) {
            System.out.println(b[i]);
        }
        //hello();
    }
}

