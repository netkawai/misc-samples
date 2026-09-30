My Azure AD(Work or School Account) principal ends up below.

contoso_live.com#EXT#_contosolive.onmicrosoft.com#EXT#@contoso.onmicrosoft.com

First I made a free email when Microsoft move to Microsoft Live from Hotmail(contoso@live.com).
Then, when I created an Azure Account with the email, Microsoft generated an Azure Directory Tenant to follow that full free email address (contosolive).
Then, I decided to move a country.... so Microsoft prohibited by any measure to change the country in tenant literally, even Microsoft can't.
So I created another tenant as contoso.onmicrosoft.com. That will be the domain part of this account. 
Why a tenant always the DNS-formatted domain?
Well... original Active Directory was developed on top of LDAP/DNS technology in which DNS domain is mandatory.


So now.. that the original user  (contoso_live.com@contosolive.onmicrosoft.com) was invited into contoso.onmicrosoft.com.
Then joined as above principal (full) account name... So many EXT.

Issuer is Microsoft Account, but when you log in contoso.onmicrosoft.com, you login as contoso_live.com#EXT#_contosolive.onmicrosoft.com#EXT#
That is NOT Microsoft Account, that is Work or School account which has the same email address with Microsoft Account. 
It means, you can still use Microsoft Account Email Address as Work or School Account which breaks many things for Microsoft 365 or Azure Devops...
because Microsoft does not provide to add Microsoft Account in Work or School on the surface. but it is not prohibited unlike changing the country in a tenant.


If you need to see Microsoft Account (contoso@live.com), access https://account.microsoft.com/
If you need to see Work Or School Account, access https://myaccount.microsoft.com/
(my is important)

This happens easily. Some developers believe that an email address will have enough uniqueness to store in a database.Yes, if you only think ONE database, it is true, but nowadays, we are facing multiple instances (database or service). If you migrate two databases or services into one, if both has the same email address as identifier, it will be in trouble.

https://graph.microsoft.com/v1.0/me/drive/
https://graph.microsoft.com/v1.0/me/drive/root:/{$path_to_item}:/createUploadSession

Create an app specific-password https://account.activedirectory.windowsazure.com/AppPasswords.aspx