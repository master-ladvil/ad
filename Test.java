public class Test{
    static {
        System.load("C:/Program Files/Java/jdk-18.0.2/bin/Test.dll");
    }
    public native void testt(String uname,String pass);
    public static void main(String args[]){
        Test te = new Test();
        te.testt("@test.local","admin@Admin");
        
    }
}
