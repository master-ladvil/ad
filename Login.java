import java.io.BufferedReader;
import java.io.IOException;
import javax.servlet.http.*;
import java.io.*;
import java.net.http.HttpResponse;;
import javax.servlet.ServletException;

public class Login extends HttpServlet{
    public static String username;
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException,IOException{
        response.setContentType("text/plain");
        PrintWriter out = response.getWriter();
        username = request.getParameter("uname");
        String pass = request.getParameter("pass");
        ActiveDirectory ad = new ActiveDirectory();
        int flag = ad.testt(username, pass);
        response.addHeader("Access-Control-Allow-Origin", "*");
        if(flag != 1){
            out.println("failure");
        }else{
            long changepwd = ad.passWordLastSet(username);
            System.out.println("Pwd flag -> "  + changepwd);
            if (changepwd == 0){
                System.out.println("need to change pwd");
                out.println("1");
            }else{
                System.out.println("need not change pwd");
                out.println("0");
            }
        }
    }
    public void doPost(HttpServletRequest request, HttpServletResponse response)throws ServletException,IOException{
        response.setContentType("text/plain");
        PrintWriter out = response.getWriter();
        String newpass = request.getParameter("newpass");
        ActiveDirectory ad = new ActiveDirectory();
        int flag = ad.resetPassword(username, newpass);
        response.addHeader("Access-Control-Allow-Origin", "*");
        out.println(flag);
    }
}
