<%@page import="java.net.URLDecoder"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>ModSecurity WAF for Java: Demo page</title>
    </head>
    <body style="background: #333333;">
        <div  align="center" style="width:930px; margin:0 auto; box-shadow: 5px 5px 6px #000; background: #FFFFFF;">
            <div width="930">
                <h1>
                    <img border="0" height="101" alt="ModSecurity: Open Source Web Application Firewall" src="http://www.modsecurity.org/g/header-top.jpg" />
                </h1>
            </div>
            <div width="930">
                <table width="90%" cellspacing="0" cellpadding="0" border="0">
                    <tr>
                        <td>
                            <h2 style="font-family: Arial;">ModSecurity Core Rule Set (CRS) - Installed demo</h2>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p>
                                Please feel free to inject malicious input to stress test the ModSecurity Core Rule Set (CRS). The form accepts both GET and POST request methods. You can either do this via the form below or manually.
                            </p>
                            <p>
                                Check your servlet context logging for ModSecurity output. The request may also be blocked if, for example, <i>SecRuleEngine</i> is <i>On</i>.
                            </p>
                            <p>
                                You can also access the <a href="help.html">ModSecurity for Java - Help page</a>.
                            </p>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <form id="demoForm" method="get" action="index.jsp">
                                <b>Payload:</b>
                                <fieldset>
                                    <textarea name="test" rows="6" cols="90" style="max-width:800px;"></textarea> <!--Foo' or '2' < '1' ;--example payload-->
                                </fieldset>

                                <fieldset><input id="submit" type="submit" value="Send"></input>
                                    method=
                                    <a id="demoMethodToggle" href="javascript:toggleMethod()"> GET </a>
                                    enctype=
                                    <a id="demoEncToggle" href="javascript:toggleEncType()"> application/x-www-form-urlencoded </a></fieldset>
                            </form>

                            <script type="text/javascript">
                                function toggleEncType() {
                                    var f = document.getElementById('demoForm');
                                    var le = document.getElementById('demoEncToggle');
                                    var lm = document.getElementById('demoMethodToggle');
                                    if (f.getAttribute('enctype') === 'application/x-www-form-urlencoded') {
                                        f.setAttribute('enctype', 'multipart/form-data');
                                        f.setAttribute('method', 'POST');
                                        le.innerHTML = 'multipart/form-data';
                                        lm.innerHTML = 'POST';
                                    }
                                    else {
                                        f.setAttribute('enctype', 'application/x-www-form-urlencoded');
                                        le.innerHTML = 'application/x-www-form-urlencoded';
                                    }
                                }
                                function toggleMethod() {
                                    var f = document.getElementById('demoForm');
                                    var le = document.getElementById('demoEncToggle');
                                    var lm = document.getElementById('demoMethodToggle');
                                    if (f.getAttribute('method') === 'POST') {
                                        f.setAttribute('enctype', 'application/x-www-form-urlencoded');
                                        f.setAttribute('method', 'GET');
                                        le.innerHTML = 'application/x-www-form-urlencoded';
                                        lm.innerHTML = 'GET';
                                    }
                                    else {
                                        f.setAttribute('method', 'POST');
                                        lm.innerHTML = 'POST';
                                    }
                                }
                            </script>

                            <br />
                            <br />
                            <br />
                            <br />
                        </td>
                    </tr>

                    <tr>
                        <td> 
                            <% if (request.getParameter("test") != null) {%>
                            <h3>Last submitted payload:</h3>
                            <p><%= request.getParameter("test") %></p>
                            <br />
                            <% }%>
                        </td>
                    </tr>
                </table>
            </div>
        </div>

    </body>
</html>
