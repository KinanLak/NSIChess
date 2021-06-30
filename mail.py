f = open("C:/Users/Kinan/Desktop/text2.txt","w+")

import smtplib
from email.message import EmailMessage 
import csv

infos = []
with open ('id.csv', 'r') as f:
        temp = csv.DictReader(f,delimiter = ';')
        for row in temp:
            infos.append(dict(row))

email_adress = 'nsichess@gmail.com'
email_mdp = 'qX58rn^qwXeA&o1k=s?Hf2!aFdF?RWdA'

msg = EmailMessage()
msg['Subject'] = "Confirmation d'inscription"
msg['From'] = 'NSI Chess'
msg['To'] = 'kinan.lakh@gmail.com'

msg.set_content("This is a plain text email")

msg.add_alternative("""
                    
<!DOCTYPE html>
<html>

<head>
    <title>Logames : confirmation</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <style type="text/css">
        /* CLIENT-SPECIFIC STYLES */
        body,
        table,
        td,
        a {
            -webkit-text-size-adjust: 100%;
            -ms-text-size-adjust: 100%;
        }

        /* Prevent WebKit and Windows mobile changing default text sizes */
        table,
        td {
            mso-table-lspace: 0pt;
            mso-table-rspace: 0pt;
        }

        /* Remove spacing between tables in Outlook 2007 and up */
        img {
            -ms-interpolation-mode: bicubic;
        }

        /* Allow smoother rendering of resized image in Internet Explorer */
        /* RESET STYLES */
        img {
            border: 0;
            height: auto;
            line-height: 100%;
            outline: none;
            text-decoration: none;
        }

        table {
            border-collapse: collapse !important;
        }

        body {
            height: 100% !important;
            margin: 0 !important;
            padding: 0 !important;
            width: 100% !important;
        }

        /* iOS BLUE LINKS */
        a[x-apple-data-detectors] {
            color: inherit !important;
            text-decoration: none !important;
            font-size: inherit !important;
            font-family: inherit !important;
            font-weight: inherit !important;
            line-height: inherit !important;
        }

        /* MOBILE STYLES */
        @media screen and (max-width: 525px) {

            /* ALLOWS FOR FLUID TABLES */
            .wrapper {
                width: 100% !important;
                max-width: 100% !important;
            }

            /* ADJUSTS LAYOUT OF LOGO IMAGE */
            .logo img {
                margin: 0 auto !important;
            }

            /* USE THESE CLASSES TO HIDE CONTENT ON MOBILE */
            .mobile-hide {
                display: none !important;
            }

            .img-max {
                max-width: 100% !important;
                width: 100% !important;
                height: auto !important;
            }

            /* FULL-WIDTH TABLES */
            .responsive-table {
                width: 100% !important;
            }

            /* UTILITY CLASSES FOR ADJUSTING PADDING ON MOBILE */
            .padding {
                padding: 10px 5% 15px 5% !important;
            }

            .padding-meta {
                padding: 30px 5% 0px 5% !important;
                text-align: center;
            }

            .no-padding {
                padding: 0 !important;
            }

            .section-padding {
                padding: 50px 15px 50px 15px !important;
            }

            /* ADJUST BUTTONS ON MOBILE */
            .mobile-button-container {
                margin: 0 auto;
                width: 100% !important;
            }

            .mobile-button {
                padding: 15px !important;
                border: 0 !important;
                font-size: 16px !important;
                display: block !important;
            }
        }

        /* ANDROID CENTER FIX */
        div[style*="margin: 16px 0;"] {
            margin: 0 !important;
        }
    </style>
</head>

<body style="margin: 0px !important; padding: 0px !important;">
    <!-- HIDDEN PREHEADER TEXT -->
    <div style="display: none; font-size: 1px; color: #fefefe; line-height: 1px; font-family: Helvetica, Arial, sans-serif; max-height: 0px; max-width: 0px; opacity: 0; overflow: hidden;">Entice *** open with some amazing preheader text. Use a little mystery *** *** those subscribers to read through...</div><!-- HEADER -->
    <table border="0" cellpadding="0" cellspacing="0" width="100%">
        <tbody>
            <tr>
                <td align="center" bgcolor="#333333">
                    <!--[if (gte mso 9)|(IE)]>
            <table align="center" border="0" cellspacing="0" cellpadding="0" width="500">
            <tr>
            <td align="center" valign="top" width="500">
            <![endif]-->
                    <table border="0" cellpadding="0" cellspacing="0" class="wrapper" style="max-width: 500px;" width="100%">
                        <tbody>
                            <tr>
                                <td align="center" class="logo" style="padding: 15px 0px; vertical-align: middle;" valign="top"><a href="https://myfiles.space/user_files/89278_dfa3d374da5c2f11/89278_custom_files/img1623663786.png" target="_blank"><img src="https://myfiles.space/user_files/89278_dfa3d374da5c2f11/89278_custom_files/img1623663786.png" style="width: 168px;"></a></td>
                            </tr>
                        </tbody>
                    </table>
                    <!--[if (gte mso 9)|(IE)]>
            </td>
            </tr>
            </table>
            <![endif]-->
                </td>
            </tr>
            <tr>
                <td align="center" bgcolor="#ffffff" class="section-padding" style="padding: 70px 15px 70px 15px;">
                    <!--[if (gte mso 9)|(IE)]>
            <table align="center" border="0" cellspacing="0" cellpadding="0" width="500">
            <tr>
            <td align="center" valign="top" width="500">
            <![endif]-->
                    <table border="0" cellpadding="0" cellspacing="0" class="responsive-table" style="max-width: 500px;" width="100%">
                        <tbody>
                            <tr>
                                <td>
                                    <!-- HERO IMAGE -->
                                    <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                        <tbody>
                                            <tr>
                                                <td align="center" class="padding"><a href="https://wallpaperaccess.com/full/1994926.jpg" target="_blank"><img src="https://wallpaperaccess.com/full/1994926.jpg" width="500" height="400" border="0" style="margin: 0 auto 0 0; display: block; text-align:left; color: rgb(102, 102, 102); font-family: Helvetica, arial, sans-serif; font-size: 16px;" class="img-max"></a></td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    <!-- COPY -->
                                                    <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                                        <tbody>
                                                            <tr>
                                                                <td align="center" class="padding" style="font-size: 25px; font-family: Helvetica, Arial, sans-serif; color: #333333; padding-top: 30px;">Bonjour """+infos[0]['prenom']+""",</td>
                                                            </tr>
                                                            <tr>
                                                                <td align="center" class="padding " style="padding: 20px 0 0 0; font-size: 16px; line-height: 25px; font-family: Helvetica, Arial, sans-serif; color: #666666;">Il semblerait que vous soyez en train de vous inscrire sur notre application. Voici votre code de confirmation :</td>
                                                            </tr>
                                                        </tbody>
                                                    </table>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td align="center">
                                                    <!-- BULLETPROOF BUTTON -->
                                                    <table border="0" cellpadding="0" cellspacing="0" width="100%">
                                                        <tbody>
                                                            <tr>
                                                                <td align="center" class="padding" style="padding-top: 25px;">
                                                                    <table border="0" cellpadding="0" cellspacing="0" class="mobile-button-container">
                                                                        <tbody>
                                                                            <tr>
                                                                                <td align="center" bgcolor="#256F9C" style="border-radius: 10px;"><a class="mobile-button" style="font-size: 16px; font-family: Helvetica, Arial, sans-serif; color: #ffffff; text-decoration: none; color: #ffffff; text-decoration: none; border-radius: 10px; padding: 15px 25px; border: 1px solid #256F9C; display: inline-block;" target="_blank"><span style="font-size: 26px;">"""+infos[0]['code']+"""</span></a></td>
                                                                            </tr>
                                                                        </tbody>
                                                                    </table>
                                                                </td>
                                                            </tr>
                                                        </tbody>
                                                    </table>
                                                </td>
                                            </tr>
                                        </tbody>
                                    </table>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                    <!--[if (gte mso 9)|(IE)]>
            </td>
            </tr>
            </table>
            <![endif]-->
                </td>
            </tr>
            <tr>
                <td align="center" bgcolor="#F5F7FA" class="section-padding" style="padding: 15px 15px 15px 15px;">
                    <p style="text-align: center;"><span style="font-family: Helvetica, sans-serif; font-size: 14px;">Si vous n&apos;&ecirc;tes pas &agrave; l&apos;origine de cette requ&ecirc;te, vous pouvez ignorer ce message.&nbsp;</span></p>
                    <p style="text-align: center;"><span style="font-family: Helvetica, sans-serif; font-size: 14px;">Sinon, entrez ce code &agrave; l&apos;endroit pr&eacute;vu sur notre plateforme.</span></p>
                </td>
            </tr>
            <tr>
                <td align="center" bgcolor="#E6E9ED" class="section-padding" style="padding: 18px 1px 4px 1px;"><a href="https://twitter.com/logamesstudio"><img src="https://myfiles.space/user_files/89278_dfa3d374da5c2f11/89278_custom_files/img1623664687.png" style="width: 38px;"></a>&nbsp; &nbsp; &nbsp;<a href="https://github.com/KinanLak/NSIChess"><img src="https://myfiles.space/user_files/89278_dfa3d374da5c2f11/89278_custom_files/img1623664813.png" style="width: 40px;"></a><br><br></td>
            </tr>
        </tbody>
    </table>
</body>

</html>

""", subtype='html')


with smtplib.SMTP_SSL("smtp.gmail.com", 465) as smtp:
    smtp.login(email_adress,email_mdp)
    smtp.send_message(msg)