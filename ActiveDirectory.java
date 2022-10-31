public class ActiveDirectory{
    static {
        System.load("C:/Program Files/Java/jdk-18.0.2/bin/ActiveDirectory.dll");
    }
    public native int testt(String uname,String pass);
    public native String getname(String user);
    public native int getP(String user);
    public native long passWordLastSet(String user);
    public native int resetPassword(String user,String password);
    public static void main(String args[]){
        ActiveDirectory te = new ActiveDirectory();
        //int flag = te.testt("maran","admin@Admin");
        //System.out.println(te.getname("arun"));
        //long flag = te.passWordLastSet("arun");

        int flag = te.resetPassword("maran", "admin@Admin");
        System.out.println(flag);
        
    }
}