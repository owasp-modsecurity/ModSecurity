package org.modsecurity;

import java.io.IOException;
import java.util.UUID;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class MsHttpTransaction {

    private HttpServletRequest req;
    private HttpServletResponse res;

    private MsHttpServletRequest msReq;
    private MsHttpServletResponse msRes;
    
    private String tranID;

    public MsHttpTransaction(ServletRequest req, ServletResponse res) {
        tranID = UUID.randomUUID().toString();
        tranID = tranID.replace('-', '0');
        this.req = (HttpServletRequest)req;
        this.res = (HttpServletResponse)res;
        this.msReq = new MsHttpServletRequest(this.req);
        this.msRes = new MsHttpServletResponse(this.res);
    }

    public void destroy() throws IOException {
        msRes.destroy();
        msReq.destroy();
    }

    /**
     * @return the req
     */
    public HttpServletRequest getHttpRequest() {
        return req;
    }

    /**
     * @return the res
     */
    public HttpServletResponse getHttpResponse() {
        return res;
    }

    /**
     * @return the msReq
     */
    public MsHttpServletRequest getMsHttpRequest() {
        return msReq;
    }

    /**
     * @return the msRes
     */
    public MsHttpServletResponse getMsHttpResponse() {
        return msRes;
    }

    /**
     * @return the tranID
     */
    public String getTransactionID() {
        return tranID;
    }

}
