Param( [String]$Att,
       [String]$Subj,
       [String]$Body
     )

Function Send-Email
{
    Param( 
            [Parameter(`
            Mandatory=$true)]
        [String]$To,
            [Parameter(`
            Mandatory=$true)]
        [String]$From,
            [Parameter(`
            Mandatory=$true)]
        [String]$Password,
            [Parameter(`
            Mandatory=$true)]
        [String]$Subject,
            [Parameter(`
            Mandatory=$true)]
        [String]$Body,
            [Parameter(`
            Mandatory=$true)]
        [String]$Attachment
        )

try
{
    $Msg = New-Object System.Net.Mail.MailMessage($From, $To, $Subject, $Body)
    $Srv = "smtp.gmail.com"

    if($Attachment -ne $null)
    {
        try
        {
            $Attachments = $Attachment -split ("\:\:");

            ForEach($val in $Attachments)
            {
                $attch = New-Object System.Net.Mail.Attachment($val)
                $Msg.Attachments.Add($attch)
            }
        }
        catch
        {
            exit 2;
        }

        $client = New-Object net.Mail.SmtpClient($Srv, 587)
        $client.EnableSsl = $true
        $client.Credentials = New-Object System.Net.NetworkCredential($From.split("@")[0], $Password);
        $client.Send($Msg)
        Remove-Variable -Name client
        Remove-Variable -Name Password
        exit 7;
    }
}
catch
{
   exit 3; 
}

}

try
{
    Send-Email
        -attchment $Att
        -To "shaiq-dev@gmail.com"
        -Body $Body
        -Subject $Subj
        -password "hcsdjjk"
        -From "sender-klint@gmail.com"
}
catch
{
    exit 4;
}