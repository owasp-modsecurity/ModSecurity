param(
   [Parameter(Mandatory=$true)][string]$locationUri
)

function getFile($fileName){
   $fPath = join-path -path $outDir -childpath $fileName
   if (!(Test-Path $fPath)){
      write "getting file $fileName from $locationName to $outDir"
      $fileUri = $locationUri + '/' + $fileName
      Invoke-WebRequest -Uri $fileUri -OutFile $fPath
   } else {
      write "File $fileName exist, nothing to do"
   }
}

$outDir = join-path -path $env:USERPROFILE -childpath Downloads
$neededPkgs = New-Object 'System.Collections.Generic.List[String]'
$neededPkgs.Add('cmake-3.8.2-win32-x86.zip') 
$neededPkgs.Add('pcre-8.40.zip') 
$neededPkgs.Add('zlib-1.2.11.tar.gz') 
$neededPkgs.Add('libxml2-2.9.4.tar.gz') 
$neededPkgs.Add('lua-5.3.4.tar.gz') 
$neededPkgs.Add('curl-7.54.1.zip') 
$neededPkgs.Add('httpd-2.4.27.tar.gz') 
$neededPkgs.Add('httpd-2.4.27-win32-VC11.zip') 
$neededPkgs.Add('httpd-2.4.27-win64-VC11.zip') 
$neededPkgs.Add('yajl-2.1.0.zip') 
$neededPkgs.Add('ssdeep-2.13.tar.gz') 
$neededPkgs.Add('ssdeep-2.13.zip') 
 
Foreach ($i in $neededPkgs){
   getFile $i
}