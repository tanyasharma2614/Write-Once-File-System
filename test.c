#include "writeonceFS.c"


int main(){
    printf("Compilation passed!!\n\n");
    char* start = (char*)malloc(CAPACITY);
    memory_rep = start;
    createFS();
    //Mounting
    if(wo_mount("diskfile", memory_rep)!=0){
        printf("Mount test failed\n");
    }
    else{
        printf("Mount test passed\n");
    }
    int fd1 = wo_create("t2", WO_RDWR);
    if(fd1 <0){
        printf("Create test failed :(\n");
    }
    else
        printf("File created and opened!! File Handle is = %d\n",fd1);
    
    if(wo_write(fd1, "wvumxwqhpr vjywxxzjws fvyfydbjdd umxduvnoyc glskrgqemx bkiogtrkca uxlypfdobw yelzyofmxl vysndgjwlb prpylsbxgr lvpcbildvp pmjhupxmqq nydkmaysbb wiwiefbjth lkuhjahvas xxbxivogen pdsdqvrztz vkgfxepbnl cfucbwrrvp zinujpwxvr wvjwmjegws euiigjklxx kbneondgfl idvlkliagn efqjdqkrrh ranhfsmzqq ezpxfnddpt wadqvltlrs sijoajindx zkodcryiyi iqufdzxmic sqcmkbtvej tvyngzfmfr asnfdbiimk ajinaavraw gprbanubxy emcjlwufgw ydpsypklui hbildwxrzs phmejtcyxc gnlxedgdfu iuytmyirkx dowdueensn hjfxdawzom zwleshnivb jymjqtqqcu yesastidhj tsdkatdnll puogcvawvs qugaopjwjx tjqzkqwvwb wcravlquuv mcrkuudcec iccufyqzrc ygtadfmsbl epamotrgqq khqksespsq mvuscypxgs zskogpzgfr gbbdvuvddx wfyhfwzcjc ifuejhqhqh zdfopydxqi wtqvtlmvhn unseguggap xxyzcjomwp troxoquvkd khmmlmzhnb onxkvwpsor mshpdynqtu cuhiggrnqk cbawnxwnfn agpztyflpo jbbvbvzlfy neyjzjoexd tkjffzbgwn qiyomjfxga uvoemerczx cqwxfwafwi btavrrvjmf pgnnrncqwb uahqsepoxg qkldsaljsr dugofbokxr vbpexyqysj kkppkakwlf nkpqeuhwhk zxxpmrthek ihsfhacsib peouxdqovf jtimcyfujw ekjwjyfplz kvibysqaX w", 1024)==0){
        printf("Write test Passed!!!\n");
    }
    else{
        printf("Write test failed!!!\n");
    }
    
    if(wo_close(fd1)!=0){
        printf("close test failed\n");
    }
    else{
        printf("close test passed\n");
    }
     //Umounting
    if(wo_unmount(memory_rep)!=0){
        printf("Unmount test failed\n");
    }
    else{
        printf("Unmount test passed\n");
    }
    //free(memory_rep);
    //memory_rep = start;
    char* end = (char*)malloc(CAPACITY);
    memory_rep =end;
    //Mounting
    if(wo_mount("diskfile", memory_rep)!=0){
        printf("Mount test failed\n");
    }
    else{
        printf("Mounted again test passed\n");
    }
    int fd2 = wo_open("t2", WO_RDWR);
    if(fd2 <0){
        printf("Create test failed :(\n");
    }
    else
        printf("File created and opened!! File Handle is = %d\n",fd2);
    //maximization();
    //Opening a file in rdwr mode    
    /*int fd1 = wo_create("t2", WO_RDWR);
    if(fd1 <0){
        printf("Create test failed :(\n");
    }
    else
        printf("File created and opened!! File Handle is = %d\n",fd1);
    
    maximization();*/
    /*if(wo_close(fd1)!=0){
        printf("close test failed\n");
    }
    else{
        printf("close test passed\n");
    }
    
    int fd2 = wo_open("t2", WO_RDWR);
    if(fd2 <0){
        printf("Open test failed :(\n");
    }
    else
        printf("File created and opened!! File Handle is = %d\n",fd2);*/
    
    
    
    
    /*if(wo_write(fd1,"w", 1)==0){
        printf("test Passed!!!\n");
    }
    else{
        printf("test failed!!!\n");
    }*/
 
    /*if(wo_write(fd1, "wvumxwqhpr vjywxxzjws fvyfydbjdd umxduvnoyc glskrgqemx bkiogtrkca uxlypfdobw yelzyofmxl vysndgjwlb prpylsbxgr lvpcbildvp pmjhupxmqq nydkmaysbb wiwiefbjth lkuhjahvas xxbxivogen pdsdqvrztz vkgfxepbnl cfucbwrrvp zinujpwxvr wvjwmjegws euiigjklxx kbneondgfl idvlkliagn efqjdqkrrh ranhfsmzqq ezpxfnddpt wadqvltlrs sijoajindx zkodcryiyi iqufdzxmic sqcmkbtvej tvyngzfmfr asnfdbiimk ajinaavraw gprbanubxy emcjlwufgw ydpsypklui hbildwxrzs phmejtcyxc gnlxedgdfu iuytmyirkx dowdueensn hjfxdawzom zwleshnivb jymjqtqqcu yesastidhj tsdkatdnll puogcvawvs qugaopjwjx tjqzkqwvwb wcravlquuv mcrkuudcec iccufyqzrc ygtadfmsbl epamotrgqq khqksespsq mvuscypxgs zskogpzgfr gbbdvuvddx wfyhfwzcjc ifuejhqhqh zdfopydxqi wtqvtlmvhn unseguggap xxyzcjomwp troxoquvkd khmmlmzhnb onxkvwpsor mshpdynqtu cuhiggrnqk cbawnxwnfn agpztyflpo jbbvbvzlfy neyjzjoexd tkjffzbgwn qiyomjfxga uvoemerczx cqwxfwafwi btavrrvjmf pgnnrncqwb uahqsepoxg qkldsaljsr dugofbokxr vbpexyqysj kkppkakwlf nkpqeuhwhk zxxpmrthek ihsfhacsib peouxdqovf jtimcyfujw ekjwjyfplz kvibysqaX w", 1020)==0){
        printf("test Passed!!!\n");
    }
    else{
        printf("test failed!!!\n");
    }*/
    /*if(wo_write(fd1, "v", 1)==0){
        printf("test Passed!!!\n");
    }
    else{
        printf("test failed!!!\n");
    }*/

    /*int fd2 = wo_open("t3", WO_RDWR);
    if(fd2 <0){
        printf("Open tes failed :(\n");
    }
    else
        printf("File opened!! File Handle is = %d\n",fd2);

    if(wo_write(fd2, "wvumxwqhpr vjywxxzjws fvyfydbjdd umxduvnoyc glskrgqemx bkiogtrkca uxlypfdobw yelzyofmxl vysndgjwlb prpylsbxgr lvpcbildvp pmjhupxmqq nydkmaysbb wiwiefbjth lkuhjahvas xxbxivogen pdsdqvrztz vkgfxepbnl cfucbwrrvp zinujpwxvr wvjwmjegws euiigjklxx kbneondgfl idvlkliagn efqjdqkrrh ranhfsmzqq ezpxfnddpt wadqvltlrs sijoajindx zkodcryiyi iqufdzxmic sqcmkbtvej tvyngzfmfr asnfdbiimk ajinaavraw gprbanubxy emcjlwufgw ydpsypklui hbildwxrzs phmejtcyxc gnlxedgdfu iuytmyirkx dowdueensn hjfxdawzom zwleshnivb jymjqtqqcu yesastidhj tsdkatdnll puogcvawvs qugaopjwjx tjqzkqwvwb wcravlquuv mcrkuudcec iccufyqzrc ygtadfmsbl epamotrgqq khqksespsq mvuscypxgs zskogpzgfr gbbdvuvddx wfyhfwzcjc ifuejhqhqh zdfopydxqi wtqvtlmvhn unseguggap xxyzcjomwp troxoquvkd khmmlmzhnb onxkvwpsor mshpdynqtu cuhiggrnqk cbawnxwnfn agpztyflpo jbbvbvzlfy neyjzjoexd tkjffzbgwn qiyomjfxga uvoemerczx cqwxfwafwi btavrrvjmf pgnnrncqwb uahqsepoxg qkldsaljsr dugofbokxr vbpexyqysj kkppkakwlf nkpqeuhwhk zxxpmrthek ihsfhacsib peouxdqovf jtimcyfujw ekjwjyfplz kvibysqaX w", 1024)==0){
        printf("Write test Passed!!!\n");
    }
    else{
        printf("Write test failed!!!\n");
    }*/
    //char * buffer = (char*)malloc();
    /*char buffer[1024];
    if(wo_read(fd1,buffer, 1024)==0){
        printf("Read test Passed!!!\n");
    }
    else{
        printf("Read test failed!!!\n");
    }
    //close test
    if(wo_close(fd1)!=0){
        printf("close test failed\n");
    }
    else{
        printf("close test passed\n");
    }

    //Umounting
    if(wo_unmount(memory_rep)!=0){
        printf("Unmount test failed\n");
    }
    else{
        printf("Unmount test passed\n");
    }
    //Mounting
    if(wo_mount("diskfile", memory_rep)!=0){
        printf("Mount test failed\n");
    }
    else{
        printf("Mounted again test passed\n");
    }

    int fd2 = wo_open("t2", WO_RDWR);
    if(fd2 <0){
        printf("Open test failed :(\n");
    }
    else{
        printf("File created and opened!! File Handle is = %d\n",fd2);
    }
    char buffer2[1024];
    if(wo_read(fd2,buffer2, 1024)==0){
        printf("Read test Passed!!!\n");
    }
    else{
        printf("Read test failed!!!\n");
    }
    //close test
    if(wo_close(fd2)!=0){
        printf("close test failed\n");
    }
    else{
        printf("close test passed\n");
    }

    //Umounting
    if(wo_unmount(memory_rep)!=0){
        printf("Unmount test failed\n");
    }
    else{
        printf("Unmount test passed\n");
    }   
    
if(wo_mount("diskfile", memory_rep)!=0){
        printf("Mount test failed\n");
    }
    else{
        printf("Mounted again test passed\n");
    }

    int fd3 = wo_open("t2", WO_RDWR);
    if(fd3 <0){
        printf("Open test failed :(\n");
    }
    else{
        printf("File created and opened!! File Handle is = %d\n",fd2);
    }
    char buffer3[1024];
    if(wo_read(fd3,buffer3, 1024)==0){
        printf("Read test Passed!!!\n");
    }
    else{
        printf("Read test failed!!!\n");
    }
    //close test
    if(wo_close(fd3)!=0){
        printf("close test failed\n");
    }
    else{
        printf("close test passed\n");
    }*/

    //Umounting
    if(wo_unmount(memory_rep)!=0){
        printf("Unmount test failed\n");
    }
    else{
        printf("Unmount test passed\n");
    }   
    


    return 0;
}