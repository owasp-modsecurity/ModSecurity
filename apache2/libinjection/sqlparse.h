  


<!DOCTYPE html>
<html>
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# githubog: http://ogp.me/ns/fb/githubog#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>ModSecurity/apache2/libinjection/sqlparse.h at da961d4c1535d2131c1733258b3f809feeabadf5 · client9/ModSecurity · GitHub</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub" />
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub" />
    <link rel="apple-touch-icon" sizes="57x57" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="114x114" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="72x72" href="/apple-touch-icon-144.png" />
    <link rel="apple-touch-icon" sizes="144x144" href="/apple-touch-icon-144.png" />
    <link rel="logo" type="image/svg" href="http://github-media-downloads.s3.amazonaws.com/github-logo.svg" />
    <link rel="xhr-socket" href="/_sockets">
    <meta name="msapplication-TileImage" content="/windows-tile.png">
    <meta name="msapplication-TileColor" content="#ffffff">

    
    
    <link rel="icon" type="image/x-icon" href="/favicon.ico" />

    <meta content="authenticity_token" name="csrf-param" />
<meta content="ZrZ9aYJ6VXtOXWla1nbvmH/GlyHS3Izy3d4RrEZU1Lg=" name="csrf-token" />

    <link href="https://a248.e.akamai.net/assets.github.com/assets/github-3dae77b1344d5de8d1c932d28c3904033713ed8b.css" media="all" rel="stylesheet" type="text/css" />
    <link href="https://a248.e.akamai.net/assets.github.com/assets/github2-c15137b0b05c94db05fa047ecd589d7a7df41d85.css" media="all" rel="stylesheet" type="text/css" />
    


      <script src="https://a248.e.akamai.net/assets.github.com/assets/frameworks-010d500708696b4ecee44478b5229d626367e844.js" type="text/javascript"></script>
      <script src="https://a248.e.akamai.net/assets.github.com/assets/github-3faa7311e682eb5f92401a6c1295f98d3fc9eaa8.js" type="text/javascript"></script>
      
      <meta http-equiv="x-pjax-version" content="58769555cfd6f51aea069970c0fad714">

        <link data-pjax-transient rel='permalink' href='/client9/ModSecurity/blob/da961d4c1535d2131c1733258b3f809feeabadf5/apache2/libinjection/sqlparse.h'>
    <meta property="og:title" content="ModSecurity"/>
    <meta property="og:type" content="githubog:gitrepository"/>
    <meta property="og:url" content="https://github.com/client9/ModSecurity"/>
    <meta property="og:image" content="https://secure.gravatar.com/avatar/a33ae5e2dab2c5f6107c878798f64694?s=420&amp;d=https://a248.e.akamai.net/assets.github.com%2Fimages%2Fgravatars%2Fgravatar-user-420.png"/>
    <meta property="og:site_name" content="GitHub"/>
    <meta property="og:description" content="ModSecurity is an open source, cross platform web application firewall (WAF) engine for Apache, IIS and Nginx that is developed by Trustwave&#39;s SpiderLabs. It has a robust event-based programming language which provides protection from a range of attacks against web applications and allows for HTTP traffic monitoring, logging and real-time analysis. With over 10,000 deployments world-wide, ModSecurity is the most widely deployed WAF in existence. "/>
    <meta property="twitter:card" content="summary"/>
    <meta property="twitter:site" content="@GitHub">
    <meta property="twitter:title" content="client9/ModSecurity"/>

    <meta name="description" content="ModSecurity is an open source, cross platform web application firewall (WAF) engine for Apache, IIS and Nginx that is developed by Trustwave&#39;s SpiderLabs. It has a robust event-based programming language which provides protection from a range of attacks against web applications and allows for HTTP traffic monitoring, logging and real-time analysis. With over 10,000 deployments world-wide, ModSecurity is the most widely deployed WAF in existence. " />

      <meta name="robots" content="noindex, nofollow">
  <link href="https://github.com/client9/ModSecurity/commits/da961d4c1535d2131c1733258b3f809feeabadf5.atom" rel="alternate" title="Recent Commits to ModSecurity:da961d4c1535d2131c1733258b3f809feeabadf5" type="application/atom+xml" />

  </head>


  <body class="logged_out page-blob  vis-public fork env-production  ">
    <div id="wrapper">

      

      
      
      

      
      <div class="header header-logged-out">
  <div class="container clearfix">

      <a class="header-logo-wordmark" href="https://github.com/">Github</a>

    <div class="header-actions">
        <a class="button primary" href="https://github.com/signup">Sign up for free</a>
      <a class="button" href="https://github.com/login?return_to=%2Fclient9%2FModSecurity%2Fblob%2Fda961d4c1535d2131c1733258b3f809feeabadf5%2Fapache2%2Flibinjection%2Fsqlparse.h">Sign in</a>
    </div>

      <ul class="top-nav">
          <li class="explore"><a href="https://github.com/explore">Explore GitHub</a></li>
        <li class="search"><a href="https://github.com/search">Search</a></li>
        <li class="features"><a href="https://github.com/features">Features</a></li>
          <li class="blog"><a href="https://github.com/blog">Blog</a></li>
      </ul>

  </div>
</div>


      

      


            <div class="site hfeed" itemscope itemtype="http://schema.org/WebPage">
      <div class="hentry">
        
        <div class="pagehead repohead instapaper_ignore readability-menu ">
          <div class="container">
            <div class="title-actions-bar">
              


<ul class="pagehead-actions">



    <li>
      <a href="/login?return_to=%2Fclient9%2FModSecurity"
        class="minibutton js-toggler-target star-button entice tooltipped upwards"
        title="You must be signed in to use this feature" rel="nofollow">
        <span class="mini-icon mini-icon-star"></span>Star
      </a>
      <a class="social-count js-social-count" href="/client9/ModSecurity/stargazers">
        0
      </a>
    </li>
    <li>
      <a href="/login?return_to=%2Fclient9%2FModSecurity"
        class="minibutton js-toggler-target fork-button entice tooltipped upwards"
        title="You must be signed in to fork a repository" rel="nofollow">
        <span class="mini-icon mini-icon-fork"></span>Fork
      </a>
      <a href="/client9/ModSecurity/network" class="social-count">
        43
      </a>
    </li>
</ul>

              <h1 itemscope itemtype="http://data-vocabulary.org/Breadcrumb" class="entry-title public">
                <span class="repo-label"><span>public</span></span>
                <span class="mega-icon mega-icon-public-repo"></span>
                <span class="author vcard">
                  <a href="/client9" class="url fn" itemprop="url" rel="author">
                  <span itemprop="title">client9</span>
                  </a></span> /
                <strong><a href="/client9/ModSecurity" class="js-current-repository">ModSecurity</a></strong>
                  <span class="fork-flag">
                    <span class="text">forked from <a href="/SpiderLabs/ModSecurity">SpiderLabs/ModSecurity</a></span>
                  </span>
              </h1>
            </div>

            
  <ul class="tabs">
      <li class="pulse-nav"><a href="/client9/ModSecurity/pulse" highlight="pulse" rel="nofollow"><span class="mini-icon mini-icon-pulse"></span></a></li>
    <li><a href="/client9/ModSecurity" class="selected" highlight="repo_source repo_downloads repo_commits repo_tags repo_branches">Code</a></li>
    <li><a href="/client9/ModSecurity/network" highlight="repo_network">Network</a></li>
    <li><a href="/client9/ModSecurity/pulls" highlight="repo_pulls">Pull Requests <span class='counter'>0</span></a></li>




    <li><a href="/client9/ModSecurity/graphs" highlight="repo_graphs repo_contributors">Graphs</a></li>


  </ul>
  
<div class="tabnav">

  <span class="tabnav-right">
    <ul class="tabnav-tabs">
          <li><a href="/client9/ModSecurity/tags" class="tabnav-tab" highlight="repo_tags">Tags <span class="counter ">3</span></a></li>
    </ul>
    
  </span>

  <div class="tabnav-widget scope">


    <div class="select-menu js-menu-container js-select-menu js-branch-menu">
      <a class="minibutton select-menu-button js-menu-target" data-hotkey="w" data-ref="">
        <span class="mini-icon mini-icon-tree"></span>
        <i>tree:</i>
        <span class="js-select-button">da961d4c15</span>
      </a>

      <div class="select-menu-modal-holder js-menu-content js-navigation-container">

        <div class="select-menu-modal">
          <div class="select-menu-header">
            <span class="select-menu-title">Switch branches/tags</span>
            <span class="mini-icon mini-icon-remove-close js-menu-close"></span>
          </div> <!-- /.select-menu-header -->

          <div class="select-menu-filters">
            <div class="select-menu-text-filter">
              <input type="text" id="commitish-filter-field" class="js-filterable-field js-navigation-enable" placeholder="Filter branches/tags">
            </div>
            <div class="select-menu-tabs">
              <ul>
                <li class="select-menu-tab">
                  <a href="#" data-tab-filter="branches" class="js-select-menu-tab">Branches</a>
                </li>
                <li class="select-menu-tab">
                  <a href="#" data-tab-filter="tags" class="js-select-menu-tab">Tags</a>
                </li>
              </ul>
            </div><!-- /.select-menu-tabs -->
          </div><!-- /.select-menu-filters -->

          <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket css-truncate" data-tab-filter="branches">

            <div data-filterable-for="commitish-filter-field" data-filterable-type="substring">

                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/master/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="master" rel="nofollow" title="master">master</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/2.1.0/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/2.1.0" rel="nofollow" title="remotes/2.1.0">remotes/2.1.0</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/2.1.0-rc7/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/2.1.0-rc7" rel="nofollow" title="remotes/2.1.0-rc7">remotes/2.1.0-rc7</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/2.1.x/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/2.1.x" rel="nofollow" title="remotes/2.1.x">remotes/2.1.x</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/2.2.x/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/2.2.x" rel="nofollow" title="remotes/2.2.x">remotes/2.2.x</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/2.5.x/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/2.5.x" rel="nofollow" title="remotes/2.5.x">remotes/2.5.x</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/2.6.x/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/2.6.x" rel="nofollow" title="remotes/2.6.x">remotes/2.6.x</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/2.7.x/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/2.7.x" rel="nofollow" title="remotes/2.7.x">remotes/2.7.x</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.0/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.0" rel="nofollow" title="remotes/tags/2.1.0">remotes/tags/2.1.0</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.0-rc7/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.0-rc7" rel="nofollow" title="remotes/tags/2.1.0-rc7">remotes/tags/2.1.0-rc7</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.1" rel="nofollow" title="remotes/tags/2.1.1">remotes/tags/2.1.1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.1-dev2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.1-dev2" rel="nofollow" title="remotes/tags/2.1.1-dev2">remotes/tags/2.1.1-dev2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.1-dev3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.1-dev3" rel="nofollow" title="remotes/tags/2.1.1-dev3">remotes/tags/2.1.1-dev3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.1-dev4/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.1-dev4" rel="nofollow" title="remotes/tags/2.1.1-dev4">remotes/tags/2.1.1-dev4</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.1-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.1-rc1" rel="nofollow" title="remotes/tags/2.1.1-rc1">remotes/tags/2.1.1-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.1-rc2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.1-rc2" rel="nofollow" title="remotes/tags/2.1.1-rc2">remotes/tags/2.1.1-rc2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.2" rel="nofollow" title="remotes/tags/2.1.2">remotes/tags/2.1.2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.2-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.2-rc1" rel="nofollow" title="remotes/tags/2.1.2-rc1">remotes/tags/2.1.2-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.2-rc2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.2-rc2" rel="nofollow" title="remotes/tags/2.1.2-rc2">remotes/tags/2.1.2-rc2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.2-rc3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.2-rc3" rel="nofollow" title="remotes/tags/2.1.2-rc3">remotes/tags/2.1.2-rc3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.3" rel="nofollow" title="remotes/tags/2.1.3">remotes/tags/2.1.3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.3-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.3-rc1" rel="nofollow" title="remotes/tags/2.1.3-rc1">remotes/tags/2.1.3-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.3-rc2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.3-rc2" rel="nofollow" title="remotes/tags/2.1.3-rc2">remotes/tags/2.1.3-rc2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.4/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.4" rel="nofollow" title="remotes/tags/2.1.4">remotes/tags/2.1.4</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.4-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.4-rc1" rel="nofollow" title="remotes/tags/2.1.4-rc1">remotes/tags/2.1.4-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.4-rc2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.4-rc2" rel="nofollow" title="remotes/tags/2.1.4-rc2">remotes/tags/2.1.4-rc2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.4-rc3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.4-rc3" rel="nofollow" title="remotes/tags/2.1.4-rc3">remotes/tags/2.1.4-rc3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.4-rc4/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.4-rc4" rel="nofollow" title="remotes/tags/2.1.4-rc4">remotes/tags/2.1.4-rc4</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.5/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.5" rel="nofollow" title="remotes/tags/2.1.5">remotes/tags/2.1.5</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.5-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.5-rc1" rel="nofollow" title="remotes/tags/2.1.5-rc1">remotes/tags/2.1.5-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.6/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.6" rel="nofollow" title="remotes/tags/2.1.6">remotes/tags/2.1.6</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.6-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.6-rc1" rel="nofollow" title="remotes/tags/2.1.6-rc1">remotes/tags/2.1.6-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.1.7/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.1.7" rel="nofollow" title="remotes/tags/2.1.7">remotes/tags/2.1.7</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.2.0-dev1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.2.0-dev1" rel="nofollow" title="remotes/tags/2.2.0-dev1">remotes/tags/2.2.0-dev1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.0/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.0" rel="nofollow" title="remotes/tags/2.5.0">remotes/tags/2.5.0</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.0-dev2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.0-dev2" rel="nofollow" title="remotes/tags/2.5.0-dev2">remotes/tags/2.5.0-dev2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.0-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.0-rc1" rel="nofollow" title="remotes/tags/2.5.0-rc1">remotes/tags/2.5.0-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.0-rc2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.0-rc2" rel="nofollow" title="remotes/tags/2.5.0-rc2">remotes/tags/2.5.0-rc2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.0-rc3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.0-rc3" rel="nofollow" title="remotes/tags/2.5.0-rc3">remotes/tags/2.5.0-rc3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.0-rc3-pre2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.0-rc3-pre2" rel="nofollow" title="remotes/tags/2.5.0-rc3-pre2">remotes/tags/2.5.0-rc3-pre2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.0-rc4/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.0-rc4" rel="nofollow" title="remotes/tags/2.5.0-rc4">remotes/tags/2.5.0-rc4</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.1" rel="nofollow" title="remotes/tags/2.5.1">remotes/tags/2.5.1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.1-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.1-rc1" rel="nofollow" title="remotes/tags/2.5.1-rc1">remotes/tags/2.5.1-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.2" rel="nofollow" title="remotes/tags/2.5.2">remotes/tags/2.5.2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.3" rel="nofollow" title="remotes/tags/2.5.3">remotes/tags/2.5.3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.4/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.4" rel="nofollow" title="remotes/tags/2.5.4">remotes/tags/2.5.4</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.5/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.5" rel="nofollow" title="remotes/tags/2.5.5">remotes/tags/2.5.5</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.6/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.6" rel="nofollow" title="remotes/tags/2.5.6">remotes/tags/2.5.6</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.7/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.7" rel="nofollow" title="remotes/tags/2.5.7">remotes/tags/2.5.7</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.7-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.7-rc1" rel="nofollow" title="remotes/tags/2.5.7-rc1">remotes/tags/2.5.7-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.8/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.8" rel="nofollow" title="remotes/tags/2.5.8">remotes/tags/2.5.8</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.9/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.9" rel="nofollow" title="remotes/tags/2.5.9">remotes/tags/2.5.9</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.10/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.10" rel="nofollow" title="remotes/tags/2.5.10">remotes/tags/2.5.10</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.10-dev1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.10-dev1" rel="nofollow" title="remotes/tags/2.5.10-dev1">remotes/tags/2.5.10-dev1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.10-dev2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.10-dev2" rel="nofollow" title="remotes/tags/2.5.10-dev2">remotes/tags/2.5.10-dev2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.10-dev3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.10-dev3" rel="nofollow" title="remotes/tags/2.5.10-dev3">remotes/tags/2.5.10-dev3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.11/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.11" rel="nofollow" title="remotes/tags/2.5.11">remotes/tags/2.5.11</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.12/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.12" rel="nofollow" title="remotes/tags/2.5.12">remotes/tags/2.5.12</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.5.13/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.5.13" rel="nofollow" title="remotes/tags/2.5.13">remotes/tags/2.5.13</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.0/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.0" rel="nofollow" title="remotes/tags/2.6.0">remotes/tags/2.6.0</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.0-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.0-rc1" rel="nofollow" title="remotes/tags/2.6.0-rc1">remotes/tags/2.6.0-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.0-rc2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.0-rc2" rel="nofollow" title="remotes/tags/2.6.0-rc2">remotes/tags/2.6.0-rc2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.1" rel="nofollow" title="remotes/tags/2.6.1">remotes/tags/2.6.1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.2" rel="nofollow" title="remotes/tags/2.6.2">remotes/tags/2.6.2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.2-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.2-rc1" rel="nofollow" title="remotes/tags/2.6.2-rc1">remotes/tags/2.6.2-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.3" rel="nofollow" title="remotes/tags/2.6.3">remotes/tags/2.6.3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.3-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.3-rc1" rel="nofollow" title="remotes/tags/2.6.3-rc1">remotes/tags/2.6.3-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.4/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.4" rel="nofollow" title="remotes/tags/2.6.4">remotes/tags/2.6.4</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.4-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.4-rc1" rel="nofollow" title="remotes/tags/2.6.4-rc1">remotes/tags/2.6.4-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.5/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.5" rel="nofollow" title="remotes/tags/2.6.5">remotes/tags/2.6.5</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.6/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.6" rel="nofollow" title="remotes/tags/2.6.6">remotes/tags/2.6.6</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.7/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.7" rel="nofollow" title="remotes/tags/2.6.7">remotes/tags/2.6.7</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.6.8/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.6.8" rel="nofollow" title="remotes/tags/2.6.8">remotes/tags/2.6.8</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.7.0/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.7.0" rel="nofollow" title="remotes/tags/2.7.0">remotes/tags/2.7.0</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.7.0-rc1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.7.0-rc1" rel="nofollow" title="remotes/tags/2.7.0-rc1">remotes/tags/2.7.0-rc1</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/tags/2.7.0-rc3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/tags/2.7.0-rc3" rel="nofollow" title="remotes/tags/2.7.0-rc3">remotes/tags/2.7.0-rc3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/remotes/trunk/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="remotes/trunk" rel="nofollow" title="remotes/trunk">remotes/trunk</a>
                </div> <!-- /.select-menu-item -->
            </div>

              <div class="select-menu-no-results">Nothing to show</div>
          </div> <!-- /.select-menu-list -->


          <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket css-truncate" data-tab-filter="tags">
            <div data-filterable-for="commitish-filter-field" data-filterable-type="substring">

                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/2.7.3/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="2.7.3" rel="nofollow" title="2.7.3">2.7.3</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/2.7.2/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="2.7.2" rel="nofollow" title="2.7.2">2.7.2</a>
                </div> <!-- /.select-menu-item -->
                <div class="select-menu-item js-navigation-item js-navigation-target ">
                  <span class="select-menu-item-icon mini-icon mini-icon-confirm"></span>
                  <a href="/client9/ModSecurity/blob/2.7.1/apache2/libinjection/sqlparse.h" class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target" data-name="2.7.1" rel="nofollow" title="2.7.1">2.7.1</a>
                </div> <!-- /.select-menu-item -->
            </div>

            <div class="select-menu-no-results">Nothing to show</div>

          </div> <!-- /.select-menu-list -->

        </div> <!-- /.select-menu-modal -->
      </div> <!-- /.select-menu-modal-holder -->
    </div> <!-- /.select-menu -->

  </div> <!-- /.scope -->

  <ul class="tabnav-tabs">
    <li><a href="/client9/ModSecurity" class="selected tabnav-tab" highlight="repo_source">Files</a></li>
    <li><a href="/client9/ModSecurity/commits/" class="tabnav-tab" highlight="repo_commits">Commits</a></li>
    <li><a href="/client9/ModSecurity/branches" class="tabnav-tab" highlight="repo_branches" rel="nofollow">Branches <span class="counter ">77</span></a></li>
  </ul>

</div>

  
  
  


            
          </div>
        </div><!-- /.repohead -->

        <div id="js-repo-pjax-container" class="container context-loader-container" data-pjax-container>
          


<!-- blob contrib key: blob_contributors:v21:fb21b56fc2bb8f0e052eeda638611d3e -->
<!-- blob contrib frag key: views10/v8/blob_contributors:v21:fb21b56fc2bb8f0e052eeda638611d3e -->


<div id="slider">
    <div class="frame-meta">

      <p title="This is a placeholder element" class="js-history-link-replace hidden"></p>

        <div class="breadcrumb">
          <span class='bold'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/client9/ModSecurity/tree/da961d4c1535d2131c1733258b3f809feeabadf5" class="js-slide-to" data-branch="da961d4c1535d2131c1733258b3f809feeabadf5" data-direction="back" itemscope="url" rel="nofollow"><span itemprop="title">ModSecurity</span></a></span></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/client9/ModSecurity/tree/da961d4c1535d2131c1733258b3f809feeabadf5/apache2" class="js-slide-to" data-branch="da961d4c1535d2131c1733258b3f809feeabadf5" data-direction="back" itemscope="url" rel="nofollow"><span itemprop="title">apache2</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/client9/ModSecurity/tree/da961d4c1535d2131c1733258b3f809feeabadf5/apache2/libinjection" class="js-slide-to" data-branch="da961d4c1535d2131c1733258b3f809feeabadf5" data-direction="back" itemscope="url" rel="nofollow"><span itemprop="title">libinjection</span></a></span><span class="separator"> / </span><strong class="final-path">sqlparse.h</strong> <span class="js-zeroclipboard zeroclipboard-button" data-clipboard-text="apache2/libinjection/sqlparse.h" data-copied-hint="copied!" title="copy to clipboard"><span class="mini-icon mini-icon-clipboard"></span></span>
        </div>

      <a href="/client9/ModSecurity/find/da961d4c1535d2131c1733258b3f809feeabadf5" class="js-slide-to" data-hotkey="t" style="display:none">Show File Finder</a>


        
  <div class="commit file-history-tease">
    <img class="main-avatar" height="24" src="https://secure.gravatar.com/avatar/a33ae5e2dab2c5f6107c878798f64694?s=140&amp;d=https://a248.e.akamai.net/assets.github.com%2Fimages%2Fgravatars%2Fgravatar-user-420.png" width="24" />
    <span class="author"><a href="/client9" rel="author">client9</a></span>
    <time class="js-relative-date" datetime="2013-04-17T06:01:20-07:00" title="2013-04-17 06:01:20">April 17, 2013</time>
    <div class="commit-title">
        <a href="/client9/ModSecurity/commit/9826f56db503cd07eed84fad292ec392ae4f05b3" class="message">libinjection sync, add doco</a>
    </div>

    <div class="participation">
      <p class="quickstat"><a href="#blob_contributors_box" rel="facebox"><strong>1</strong> contributor</a></p>
      
    </div>
    <div id="blob_contributors_box" style="display:none">
      <h2>Users on GitHub who have contributed to this file</h2>
      <ul class="facebox-user-list">
        <li>
          <img height="24" src="https://secure.gravatar.com/avatar/a33ae5e2dab2c5f6107c878798f64694?s=140&amp;d=https://a248.e.akamai.net/assets.github.com%2Fimages%2Fgravatars%2Fgravatar-user-420.png" width="24" />
          <a href="/client9">client9</a>
        </li>
      </ul>
    </div>
  </div>


    </div><!-- ./.frame-meta -->

    <div class="frames">
      <div class="frame" data-permalink-url="/client9/ModSecurity/blob/da961d4c1535d2131c1733258b3f809feeabadf5/apache2/libinjection/sqlparse.h" data-title="ModSecurity/apache2/libinjection/sqlparse.h at da961d4c1535d2131c1733258b3f809feeabadf5 · client9/ModSecurity · GitHub" data-type="blob">

        <div id="files" class="bubble">
          <div class="file">
            <div class="meta">
              <div class="info">
                <span class="icon"><b class="mini-icon mini-icon-text-file"></b></span>
                <span class="mode" title="File Mode">file</span>
                  <span>84 lines (69 sloc)</span>
                <span>1.586 kb</span>
              </div>
              <div class="actions">
                <div class="button-group">
                      <a class="minibutton js-entice" href=""
                         data-entice="You must be signed in and on a branch to make or propose changes">Edit</a>
                  <a href="/client9/ModSecurity/raw/da961d4c1535d2131c1733258b3f809feeabadf5/apache2/libinjection/sqlparse.h" class="button minibutton " id="raw-url">Raw</a>
                    <a href="/client9/ModSecurity/blame/da961d4c1535d2131c1733258b3f809feeabadf5/apache2/libinjection/sqlparse.h" class="button minibutton ">Blame</a>
                  <a href="/client9/ModSecurity/commits/da961d4c1535d2131c1733258b3f809feeabadf5/apache2/libinjection/sqlparse.h" class="button minibutton " rel="nofollow">History</a>
                </div><!-- /.button-group -->
              </div><!-- /.actions -->

            </div>
                <div class="blob-wrapper data type-c js-blob-data">
      <table class="file-code file-diff">
        <tr class="file-code-line">
          <td class="blob-line-nums">
            <span id="L1" rel="#L1">1</span>
<span id="L2" rel="#L2">2</span>
<span id="L3" rel="#L3">3</span>
<span id="L4" rel="#L4">4</span>
<span id="L5" rel="#L5">5</span>
<span id="L6" rel="#L6">6</span>
<span id="L7" rel="#L7">7</span>
<span id="L8" rel="#L8">8</span>
<span id="L9" rel="#L9">9</span>
<span id="L10" rel="#L10">10</span>
<span id="L11" rel="#L11">11</span>
<span id="L12" rel="#L12">12</span>
<span id="L13" rel="#L13">13</span>
<span id="L14" rel="#L14">14</span>
<span id="L15" rel="#L15">15</span>
<span id="L16" rel="#L16">16</span>
<span id="L17" rel="#L17">17</span>
<span id="L18" rel="#L18">18</span>
<span id="L19" rel="#L19">19</span>
<span id="L20" rel="#L20">20</span>
<span id="L21" rel="#L21">21</span>
<span id="L22" rel="#L22">22</span>
<span id="L23" rel="#L23">23</span>
<span id="L24" rel="#L24">24</span>
<span id="L25" rel="#L25">25</span>
<span id="L26" rel="#L26">26</span>
<span id="L27" rel="#L27">27</span>
<span id="L28" rel="#L28">28</span>
<span id="L29" rel="#L29">29</span>
<span id="L30" rel="#L30">30</span>
<span id="L31" rel="#L31">31</span>
<span id="L32" rel="#L32">32</span>
<span id="L33" rel="#L33">33</span>
<span id="L34" rel="#L34">34</span>
<span id="L35" rel="#L35">35</span>
<span id="L36" rel="#L36">36</span>
<span id="L37" rel="#L37">37</span>
<span id="L38" rel="#L38">38</span>
<span id="L39" rel="#L39">39</span>
<span id="L40" rel="#L40">40</span>
<span id="L41" rel="#L41">41</span>
<span id="L42" rel="#L42">42</span>
<span id="L43" rel="#L43">43</span>
<span id="L44" rel="#L44">44</span>
<span id="L45" rel="#L45">45</span>
<span id="L46" rel="#L46">46</span>
<span id="L47" rel="#L47">47</span>
<span id="L48" rel="#L48">48</span>
<span id="L49" rel="#L49">49</span>
<span id="L50" rel="#L50">50</span>
<span id="L51" rel="#L51">51</span>
<span id="L52" rel="#L52">52</span>
<span id="L53" rel="#L53">53</span>
<span id="L54" rel="#L54">54</span>
<span id="L55" rel="#L55">55</span>
<span id="L56" rel="#L56">56</span>
<span id="L57" rel="#L57">57</span>
<span id="L58" rel="#L58">58</span>
<span id="L59" rel="#L59">59</span>
<span id="L60" rel="#L60">60</span>
<span id="L61" rel="#L61">61</span>
<span id="L62" rel="#L62">62</span>
<span id="L63" rel="#L63">63</span>
<span id="L64" rel="#L64">64</span>
<span id="L65" rel="#L65">65</span>
<span id="L66" rel="#L66">66</span>
<span id="L67" rel="#L67">67</span>
<span id="L68" rel="#L68">68</span>
<span id="L69" rel="#L69">69</span>
<span id="L70" rel="#L70">70</span>
<span id="L71" rel="#L71">71</span>
<span id="L72" rel="#L72">72</span>
<span id="L73" rel="#L73">73</span>
<span id="L74" rel="#L74">74</span>
<span id="L75" rel="#L75">75</span>
<span id="L76" rel="#L76">76</span>
<span id="L77" rel="#L77">77</span>
<span id="L78" rel="#L78">78</span>
<span id="L79" rel="#L79">79</span>
<span id="L80" rel="#L80">80</span>
<span id="L81" rel="#L81">81</span>
<span id="L82" rel="#L82">82</span>
<span id="L83" rel="#L83">83</span>

          </td>
          <td class="blob-line-code">
                  <div class="highlight"><pre><div class='line' id='LC1'><span class="cm">/**</span></div><div class='line' id='LC2'><span class="cm"> * Copyright 2012, Nick Galbreath</span></div><div class='line' id='LC3'><span class="cm"> * nickg@client9.com</span></div><div class='line' id='LC4'><span class="cm"> * BSD License -- see COPYING.txt for details</span></div><div class='line' id='LC5'><span class="cm"> *</span></div><div class='line' id='LC6'><span class="cm"> *</span></div><div class='line' id='LC7'><span class="cm"> * HOW TO USE:</span></div><div class='line' id='LC8'><span class="cm"> *</span></div><div class='line' id='LC9'><span class="cm"> *   // Normalize query or postvar value</span></div><div class='line' id='LC10'><span class="cm"> *   // ATTENTION: this modifies user_string... make copy if that is not ok</span></div><div class='line' id='LC11'><span class="cm"> *   size_t new_len = qs_normalize(user_string, user_string_len);</span></div><div class='line' id='LC12'><span class="cm"> *</span></div><div class='line' id='LC13'><span class="cm"> *   sfilter s;</span></div><div class='line' id='LC14'><span class="cm"> *   bool sqli = is_sqli(&amp;s, user_string, new_len);</span></div><div class='line' id='LC15'><span class="cm"> *</span></div><div class='line' id='LC16'><span class="cm"> *   // That&#39;s it!  sfilter s has some data on how it matched or not</span></div><div class='line' id='LC17'><span class="cm"> *   // details to come!</span></div><div class='line' id='LC18'><span class="cm"> *</span></div><div class='line' id='LC19'><span class="cm"> */</span></div><div class='line' id='LC20'><br/></div><div class='line' id='LC21'><span class="cp">#ifndef _SQLPARSE_H</span></div><div class='line' id='LC22'><span class="cp">#define _SQLPARSE_H</span></div><div class='line' id='LC23'><br/></div><div class='line' id='LC24'><span class="cp">#ifdef __cplusplus</span></div><div class='line' id='LC25'><span class="k">extern</span> <span class="s">&quot;C&quot;</span> <span class="p">{</span></div><div class='line' id='LC26'><span class="cp">#endif</span></div><div class='line' id='LC27'><br/></div><div class='line' id='LC28'><span class="cp">#define ST_MAX_SIZE 32</span></div><div class='line' id='LC29'><span class="cp">#define MAX_TOKENS 5</span></div><div class='line' id='LC30'><br/></div><div class='line' id='LC31'><span class="k">typedef</span> <span class="k">struct</span> <span class="p">{</span></div><div class='line' id='LC32'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">char</span> <span class="n">type</span><span class="p">;</span></div><div class='line' id='LC33'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">char</span> <span class="n">str_open</span><span class="p">;</span></div><div class='line' id='LC34'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">char</span> <span class="n">str_close</span><span class="p">;</span></div><div class='line' id='LC35'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">char</span> <span class="n">val</span><span class="p">[</span><span class="n">ST_MAX_SIZE</span><span class="p">];</span></div><div class='line' id='LC36'><span class="p">}</span> <span class="kt">stoken_t</span><span class="p">;</span></div><div class='line' id='LC37'><br/></div><div class='line' id='LC38'><span class="k">typedef</span> <span class="k">struct</span> <span class="p">{</span></div><div class='line' id='LC39'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* input */</span></div><div class='line' id='LC40'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">const</span> <span class="kt">char</span> <span class="o">*</span><span class="n">s</span><span class="p">;</span></div><div class='line' id='LC41'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">size_t</span> <span class="n">slen</span><span class="p">;</span></div><div class='line' id='LC42'><br/></div><div class='line' id='LC43'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* current tokenize state */</span></div><div class='line' id='LC44'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">size_t</span> <span class="n">pos</span><span class="p">;</span></div><div class='line' id='LC45'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span>    <span class="n">in_comment</span><span class="p">;</span></div><div class='line' id='LC46'><br/></div><div class='line' id='LC47'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* syntax fixups state */</span></div><div class='line' id='LC48'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">stoken_t</span> <span class="n">syntax_current</span><span class="p">;</span></div><div class='line' id='LC49'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">stoken_t</span> <span class="n">syntax_last</span><span class="p">;</span></div><div class='line' id='LC50'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">stoken_t</span> <span class="n">syntax_comment</span><span class="p">;</span></div><div class='line' id='LC51'><br/></div><div class='line' id='LC52'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* constant folding state */</span></div><div class='line' id='LC53'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">stoken_t</span> <span class="n">fold_current</span><span class="p">;</span></div><div class='line' id='LC54'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">stoken_t</span> <span class="n">fold_last</span><span class="p">;</span></div><div class='line' id='LC55'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">fold_state</span><span class="p">;</span></div><div class='line' id='LC56'><br/></div><div class='line' id='LC57'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/* final sqli data */</span></div><div class='line' id='LC58'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">stoken_t</span> <span class="n">tokenvec</span><span class="p">[</span><span class="n">MAX_TOKENS</span><span class="p">];</span></div><div class='line' id='LC59'><br/></div><div class='line' id='LC60'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="c1">// +1 for possible ending null</span></div><div class='line' id='LC61'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">char</span> <span class="n">pat</span><span class="p">[</span><span class="n">MAX_TOKENS</span> <span class="o">+</span> <span class="mi">1</span><span class="p">];</span></div><div class='line' id='LC62'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">char</span> <span class="n">delim</span><span class="p">;</span></div><div class='line' id='LC63'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">reason</span><span class="p">;</span></div><div class='line' id='LC64'><span class="p">}</span> <span class="n">sfilter</span><span class="p">;</span></div><div class='line' id='LC65'><br/></div><div class='line' id='LC66'><span class="cm">/**</span></div><div class='line' id='LC67'><span class="cm"> * Pointer to function, takes cstr input, return true/false</span></div><div class='line' id='LC68'><span class="cm"> */</span></div><div class='line' id='LC69'><span class="k">typedef</span> <span class="nf">int</span> <span class="p">(</span><span class="o">*</span><span class="n">ptr_fingerprints_fn</span><span class="p">)(</span><span class="k">const</span> <span class="kt">char</span><span class="o">*</span><span class="p">);</span></div><div class='line' id='LC70'><br/></div><div class='line' id='LC71'><span class="cm">/**</span></div><div class='line' id='LC72'><span class="cm"> *</span></div><div class='line' id='LC73'><span class="cm"> *</span></div><div class='line' id='LC74'><span class="cm"> * \return TRUE if SQLi, FALSE is benign</span></div><div class='line' id='LC75'><span class="cm"> */</span></div><div class='line' id='LC76'><span class="kt">int</span> <span class="nf">is_sqli</span><span class="p">(</span><span class="n">sfilter</span> <span class="o">*</span> <span class="n">sql_state</span><span class="p">,</span> <span class="k">const</span> <span class="kt">char</span> <span class="o">*</span><span class="n">s</span><span class="p">,</span> <span class="kt">size_t</span> <span class="n">slen</span><span class="p">,</span></div><div class='line' id='LC77'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">ptr_fingerprints_fn</span> <span class="n">fn</span><span class="p">);</span></div><div class='line' id='LC78'><br/></div><div class='line' id='LC79'><span class="cp">#ifdef __cplusplus</span></div><div class='line' id='LC80'><span class="p">}</span></div><div class='line' id='LC81'><span class="cp">#endif</span></div><div class='line' id='LC82'><br/></div><div class='line' id='LC83'><span class="cp">#endif </span><span class="cm">/* _SQLPARSE_H */</span><span class="cp"></span></div></pre></div>
          </td>
        </tr>
      </table>
  </div>

          </div>
        </div>

        <a href="#jump-to-line" rel="facebox" data-hotkey="l" class="js-jump-to-line" style="display:none">Jump to Line</a>
        <div id="jump-to-line" style="display:none">
          <h2>Jump to Line</h2>
          <form accept-charset="UTF-8" class="js-jump-to-line-form">
            <input class="textfield js-jump-to-line-field" type="text">
            <div class="full-button">
              <button type="submit" class="button">Go</button>
            </div>
          </form>
        </div>

      </div>
    </div>
</div>

<div id="js-frame-loading-template" class="frame frame-loading large-loading-area" style="display:none;">
  <img class="js-frame-loading-spinner" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-128.gif?1360648843" height="64" width="64">
</div>


        </div>
      </div>
      <div class="context-overlay"></div>
    </div>

      <div id="footer-push"></div><!-- hack for sticky footer -->
    </div><!-- end of wrapper - hack for sticky footer -->

      <!-- footer -->
      <div id="footer">
  <div class="container clearfix">

      <dl class="footer_nav">
        <dt>GitHub</dt>
        <dd><a href="https://github.com/about">About us</a></dd>
        <dd><a href="https://github.com/blog">Blog</a></dd>
        <dd><a href="https://github.com/contact">Contact &amp; support</a></dd>
        <dd><a href="http://enterprise.github.com/">GitHub Enterprise</a></dd>
        <dd><a href="http://status.github.com/">Site status</a></dd>
      </dl>

      <dl class="footer_nav">
        <dt>Applications</dt>
        <dd><a href="http://mac.github.com/">GitHub for Mac</a></dd>
        <dd><a href="http://windows.github.com/">GitHub for Windows</a></dd>
        <dd><a href="http://eclipse.github.com/">GitHub for Eclipse</a></dd>
        <dd><a href="http://mobile.github.com/">GitHub mobile apps</a></dd>
      </dl>

      <dl class="footer_nav">
        <dt>Services</dt>
        <dd><a href="http://get.gaug.es/">Gauges: Web analytics</a></dd>
        <dd><a href="http://speakerdeck.com">Speaker Deck: Presentations</a></dd>
        <dd><a href="https://gist.github.com">Gist: Code snippets</a></dd>
        <dd><a href="http://jobs.github.com/">Job board</a></dd>
      </dl>

      <dl class="footer_nav">
        <dt>Documentation</dt>
        <dd><a href="http://help.github.com/">GitHub Help</a></dd>
        <dd><a href="http://developer.github.com/">Developer API</a></dd>
        <dd><a href="http://github.github.com/github-flavored-markdown/">GitHub Flavored Markdown</a></dd>
        <dd><a href="http://pages.github.com/">GitHub Pages</a></dd>
      </dl>

      <dl class="footer_nav">
        <dt>More</dt>
        <dd><a href="http://training.github.com/">Training</a></dd>
        <dd><a href="https://github.com/edu">Students &amp; teachers</a></dd>
        <dd><a href="http://shop.github.com">The Shop</a></dd>
        <dd><a href="/plans">Plans &amp; pricing</a></dd>
        <dd><a href="http://octodex.github.com/">The Octodex</a></dd>
      </dl>

      <hr class="footer-divider">


    <p class="right">&copy; 2013 <span title="0.19231s from fe3.rs.github.com">GitHub</span>, Inc. All rights reserved.</p>
    <a class="left" href="https://github.com/">
      <span class="mega-icon mega-icon-invertocat"></span>
    </a>
    <ul id="legal">
        <li><a href="https://github.com/site/terms">Terms of Service</a></li>
        <li><a href="https://github.com/site/privacy">Privacy</a></li>
        <li><a href="https://github.com/security">Security</a></li>
    </ul>

  </div><!-- /.container -->

</div><!-- /.#footer -->


    <div class="fullscreen-overlay js-fullscreen-overlay" id="fullscreen_overlay">
  <div class="fullscreen-container js-fullscreen-container">
    <div class="textarea-wrap">
      <textarea name="fullscreen-contents" id="fullscreen-contents" class="js-fullscreen-contents" placeholder="" data-suggester="fullscreen_suggester"></textarea>
          <div class="suggester-container">
              <div class="suggester fullscreen-suggester js-navigation-container" id="fullscreen_suggester"
                 data-url="/client9/ModSecurity/suggestions/commit">
              </div>
          </div>
    </div>
  </div>
  <div class="fullscreen-sidebar">
    <a href="#" class="exit-fullscreen js-exit-fullscreen tooltipped leftwards" title="Exit Zen Mode">
      <span class="mega-icon mega-icon-normalscreen"></span>
    </a>
    <a href="#" class="theme-switcher js-theme-switcher tooltipped leftwards"
      title="Switch themes">
      <span class="mini-icon mini-icon-brightness"></span>
    </a>
  </div>
</div>



    <div id="ajax-error-message" class="flash flash-error">
      <span class="mini-icon mini-icon-exclamation"></span>
      Something went wrong with that request. Please try again.
      <a href="#" class="mini-icon mini-icon-remove-close ajax-error-dismiss"></a>
    </div>

    
    
    <span id='server_response_time' data-time='0.19293' data-host='fe3'></span>
    
  </body>
</html>

