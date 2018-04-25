# **** Draft ****
        

### What is ModSecurity?

ModSecurity is an open source, cross platform web application firewall (WAF) engine for Apache, IIS and Nginx that is developed by Trustwave's SpiderLabs. It has a robust event-based programming language which provides protection from a range of attacks against web applications and allows for HTTP traffic monitoring, logging and real-time analysis. ModSecurity is the most widely deployed WAF in existence. 

#### What can ModSecurity do?

The freedom to choose what to do is an essential part of ModSecurity's identity and goes very well with its open source nature. With full access to the source code, your freedom to choose extends to the ability to customize and extend the tool itself to make it fit your needs. It's not a matter of ideology, but of practicality. List of scenarios where ModSecurity play an important hole:

##### Real-time application security monitoring and access control
At its core, ModSecurity gives you access to the HTTP traffic stream, in real-time, along with the ability to inspect it. This is enough for real-time security monitoring. There's an added dimension of what's possible through ModSecurity's persistent storage mechanism, which enables you to track system elements over time and perform event correlation. You are able to reliably block, if you so wish, because ModSecurity uses full request and response buffering. 

##### Full HTTP traffic logging
Web servers traditionally do very little when it comes to logging for security purposes. They log very little by default, and even with a lot of tweaking you are not able to get everything that you need. I have yet to encounter a web server that is able to log full transaction data. ModSecurity gives you that ability to log anything you need, including raw transaction data, which is essential for forensics. In addition, you get to choose which transactions are logged, which parts of a transaction are logged, and which parts are sanitized. 

##### Continuous passive security assessment
Security assessment is largely seen as an active scheduled event, in which an independent team is sourced to try to perform a simulated attack. Continuous passive security assessment is a variation of real-time monitoring, where, instead of focusing on the behavior of the external parties, you focus on the behavior of the system itself. It's an early warning system of sorts that can detect traces of many abnormalities and security weaknesses before they are exploited. 

##### Web application hardening
One of my favorite uses for ModSecurity is attack surface reduction, in which you selectively narrow down the HTTP features you are willing to accept (e.g., request methods, request headers, content types, etc.). ModSecurity can assist you in enforcing many similar restrictions, either directly, or through collaboration with other Apache modules. They all fall under web application hardening. For example, it is possible to fix many session management issues, as well as cross-site request forgery vulnerabilities. 

##### Something small, yet very important to you
Real life often throws unusual demands to us, and that is when the flexibility of ModSecurity comes in handy where you need it the most. It may be a security need, but it may also be something completely different. For example, some people use ModSecurity as an XML web service router, combining its ability to parse XML and apply XPath expressions with its ability to proxy requests.


#### What is the difference between ModSecurity version 3.x.x and v2.x.x?

In version v3...

* All Apache dependences have been removed
* Higher performance
* New features
* New architecture

Libmodsecurity is a complete rewrite of the ModSecurity platform. When it was first devised the ModSecurity project started as just an Apache module. Over time the project has been extended, due to popular demand, to support other platforms including (but not limited to) Nginx and IIS. In order to provide for the growing demand for additional platform support, it has became necessary to remove the Apache dependencies underlying this project, making it more platform independent.

As a result of this goal we have rearchitechted Libmodsecurity such that it is no longer dependent on the Apache web server (both at compilation and during runtime). One side effect of this is that across all platforms users can expect increased performance. Additionally, we have taken this opprotunity to lay the groundwork for some new features that users have been long seeking. For example we are looking to nativly support auditlogs in the JSON format, along with a host of other functionality in future versions.


### Documentation

- https://github.com/SpiderLabs/ModSecurity/wiki/
- https://github.com/SpiderLabs/ModSecurity/wiki/Reference-Manual-%28v2.x%29


### Contributing

- https://help.github.com/articles/creating-a-pull-request/


### Rules

#### Commercial Rules from Trustwave SpiderLabs

The ModSecurity Rules from Trustwave SpiderLabs are based on intelligence gathered from real-world investigations, penetration tests and research. The rules package is updated daily by the SpiderLabs Research Team to ensure that customers receive critical updates in a timely manner.

Here is a quick listing of security coverage:
 - Virtual Patching
 - IP Reputation
 - Web-based Malware Detection
 - Webshell/Backdoor Detection
 - Botnet Attack Detection
 - HTTP Denial of Service (DoS) Attack Detection
 - Anti-Virus Scanning of File Attachments

Get to know more about [Commercial Rules from Trustwave SpiderLabs](https://ssl.trustwave.com/web-application-firewall)


#### OWASP ModSecurity Core Rule Set (CRS) Version 3

The OWASP ModSecurity CRS Project's goal is to provide an easily "pluggable" set of generic attack detection rules that provide a base level of protection for any web application.

Our latest 3.0 version provides protections if the following attack/threat categories:

 - HTTP Protocol Protection
 - Real-time Blacklist Lookups
 - HTTP Denial of Service Protections
 - Generic Web Attack Protection
 - Error Detection and Hiding


### Need help?


#### Commercial Services from Trustwave

Trustwave offers a number of commercial offerings and services to support the ModSecurity community including:

 - ModSecurity Support - Receive phone and email support from the Trustwave Technical Assistance Center (TAC) for ModSecurity configuration questions, alert analysis questions and trouble-shooting.

 - ModSecurity Professional Services - Receive hands-on assistance directly from the SpiderLabs Research ModSecurity team for installation help, advanced/custom configurations and virtual patching.

 - ModSecurity Training - The SpiderLabs Research ModSecurity Team teaches students how to defend web applications with ModSecurity including hands-on lab activities and is available online or onsite in one, two or three-day seminars.


#### Community Support


[Create a new issue](https://github.com/SpiderLabs/ModSecurity/issues/new) on our GitHub Project.

