import java.io.BufferedReader;
import java.io.IOException;
import javax.servlet.http.*;
import java.io.*;
import java.net.http.HttpResponse;;
import javax.servlet.ServletException;

public class Login extends HttpServlet{
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException,IOException{
        response.setContentType("text/plain");
        PrintWriter out = response.getWriter();
        String uname = request.getParameter("uname");
        String pass = request.getParameter("pass");
        ActiveDirectory ad = new ActiveDirectory();
        int flag = ad.testt(uname, pass);
        response.addHeader("Access-Control-Allow-Origin", "*");
        if(flag != 1){
            out.println("failure");
        }else{
            long changepwd = ad.passWordLastSet(uname);
            System.out.println("Pwd flag -> "  + changepwd);
            if (changepwd == 0){
                System.out.println("need to change pwd");
            }else{
                System.out.println("need not change pwd");
            }
            out.println("success");
        }
    }
}
