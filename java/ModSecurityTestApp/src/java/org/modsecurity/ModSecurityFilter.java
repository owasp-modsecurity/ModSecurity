package org.modsecurity;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.util.UUID;
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
 * Docs: http://docs.oracle.com/javaee/6/tutorial/doc/bnagb.html
 */
public class ModSecurityFilter implements Filter {

    ModSecurity modsecurity;

    @Override
    public void init(FilterConfig fc) throws ServletException {
        String confFilename = fc.getInitParameter("conf");
        if (confFilename == null) {
            throw new ServletException("ModSecurity: parameter 'conf' not available in web.xml");
        } else {
            confFilename = fc.getServletContext().getRealPath(confFilename);
        }

        modsecurity = new ModSecurity(fc, confFilename);
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain fc) throws IOException, ServletException {
        HttpServletRequest httpReq = (HttpServletRequest) request;
        HttpServletResponse httpResp = (HttpServletResponse) response;
        MsHttpTransaction httpTran = new MsHttpTransaction(httpReq, httpResp);

        try {
            int status = modsecurity.onRequest(modsecurity.getConfFilename(), httpTran, modsecurity.checkModifiedConfig());

            if (status != ModSecurity.DECLINED) {
                return;
            }

            //BufferedInputStream buf = new BufferedInputStream(httpReqWrapper.getInputStream());
            fc.doFilter(httpTran.getMsHttpRequest(), httpTran.getMsHttpResponse());
            //status = modsecurity.onResponse(response, httpResp, requestID);
            
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
