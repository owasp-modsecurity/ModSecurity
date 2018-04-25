
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

### Rules

### Demo

### Need help?






