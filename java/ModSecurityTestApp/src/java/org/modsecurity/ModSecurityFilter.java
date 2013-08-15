package org.modsecurity;

import java.io.IOException;
import java.net.URLDecoder;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 *
 * @author Mihai Pitu
 */
public class ModSecurityFilter implements Filter {

    ModSecurity modsecurity;

    @Override
    public void init(FilterConfig fc) throws ServletException {
        String confFilename = fc.getInitParameter("conf");
        if (confFilename == null) {
            throw new ServletException("ModSecurity: parameter 'conf' not available in web.xml");
        }

        modsecurity = new ModSecurity(fc, confFilename);
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain fc) throws IOException, ServletException {
        HttpServletRequest httpReq = (HttpServletRequest) request;
        HttpServletResponse httpResp = (HttpServletResponse) response;
        MsHttpTransaction httpTran = new MsHttpTransaction(httpReq, httpResp); //transaction object used by native code

        try { 
            int status = modsecurity.onRequest(modsecurity.getConfFilename(), httpTran, modsecurity.checkModifiedConfig()); //modsecurity reloads only if primary config file is modified

            if (status != ModSecurity.DECLINED) {
                httpTran.getHttpResponse().sendError(403);
                return;
            }
            
            //process request
            fc.doFilter(httpTran.getMsHttpRequest(), httpTran.getMsHttpResponse());


            status = modsecurity.onResponse(httpTran);

            if (status != ModSecurity.OK && status != ModSecurity.DECLINED) {
                httpTran.getMsHttpResponse().reset();
                httpTran.getMsHttpResponse().setStatus(status);
            }

        } finally {
            httpTran.destroy();
        }

    }

    @Override
    public void destroy() {
        if (modsecurity != null) {
            modsecurity.destroy();
        }
    }
}
