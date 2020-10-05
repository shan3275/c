/**************************************************************
* Copyright (C) 2014-2019 All rights reserved.
* @Version: 1.0
* @Created: 2019-07-24 22:03
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include <stdio.h>
#include <string.h>

int main()
{
    char *token;
    char *p;
    //char demo[]="http://api.weibo.cn/2/profile/dealatt?trim_level=1&networktype=wifi&cardid=0001980001_1892463935&invite=0&able_recommend=0&uicode=10000198&moduleID=700&featurecode=10000001&wb_version=3557&c=android&i=e3f2980&s=dcc5b47f&ft=0&ua=samsung-GT-I9300__weibo__8.1.1__android__android4.4.2&wm=2468_1001&aid=01AsQCFpPSpNq_4FHxpjLlZEs6HaOh9yt1F8wZ3XOuWteXmKU.&uid=1892463935&v_f=2&v_p=58&from=1081195010&gsid=_2A253dH0xDeTxGeBN41UZ-S3Nzz2IHXVSIPf5rDV6PUJbkdANLUX1kWpNRDQNKXsxRoSwjz9OGi5PC2Map7qtkAzS&imei=867075010921975&uid=2371980&lang=zh_CN&lfid=100016387893141&skin=default&trim=1&oldwm=2468_1001&sflag=1&luicode=10000001";
    char demo[]="http://api.weibo.cn/2/profile/dealatt";
    token=demo;
    strsep(&token, "?");
    while(token!=NULL)
    {
        //这里不能直接写demo
       p = strsep(&token, "&"); 
        printf("%s, len:%lu\n",p, strlen(p));
    }
    return 0;
}
