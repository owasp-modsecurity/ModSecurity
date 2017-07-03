'Dirty patch for compiling PCRE 8.40 on Windows as per:https://vcs.pcre.org/pcre/code/trunk/CMakeLists.txt?r1=1659&r2=1677&view=patch


Set oShell = CreateObject( "WScript.Shell" )
workdir=oShell.ExpandEnvironmentStrings("%WORK_DIR%")


strFile = workdir & "\pcre\CMakeLists.txt"
strFileTmp = workdir & "\pcre\CMakeLists-patched.txt"

'strFile = "c:\Users\vhora\Downloads\CMakeLists.txt"
'strFileTmp = "c:\Users\vhora\Downloads\CMakeLists-patched.txt"

Dim fso
Set fso = WScript.CreateObject("Scripting.Filesystemobject")
Set f = fso.CreateTextFile(strFileTmp, 2)
Set objFS = CreateObject("Scripting.FileSystemObject")
Set objFile = objFS.OpenTextFile(strFile)

Do Until objFile.AtEndOfStream
    strLine = objFile.ReadLine
    If InStr(strLine,"OPTION(PCRE_STATIC_RUNTIME OFF CACHE BOOL")> 0 Then
        strLine = Replace(strLine,"OPTION(PCRE_STATIC_RUNTIME OFF CACHE BOOL","OPTION(PCRE_STATIC_RUNTIME")
    End If 
    f.WriteLine strLine
Loop

f.Close


objFile.Close
objFS.DeleteFile(strFile)
objFS.MoveFile strFileTmp, strFile