#!/bin/tcsh -fv

# http://wiki.mikrotik.com/wiki/Super_Mario_Theme
# hacked to c by peb */


#cat << END | mysql -sr -upaul -ppaul paul
cat << END | mysql -sr -upaul -ppaulDev pauldev
    drop table if exists mario
    ;

    create table mario (
       id int not NULL auto_increment,
       freq int,
       dur int,
       index k_id (id)
    ) engine=myisam
    ;

    insert into mario select NULL,660, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,660, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,660, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,510, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,660, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,770, 100 ; 
    insert into mario select NULL,0, 550 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 575 ; 
    insert into mario select NULL,510, 100 ; 
    insert into mario select NULL,0, 450 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 400 ; 
    insert into mario select NULL,320, 100 ; 
    insert into mario select NULL,0, 500 ; 
    insert into mario select NULL,440, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,480, 80 ; 
    insert into mario select NULL,0, 330 ; 
    insert into mario select NULL,450, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 200 ; 
    insert into mario select NULL,660, 80 ; 
    insert into mario select NULL,0, 200 ; 
    insert into mario select NULL,760, 50 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,860, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,700, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,760, 50 ; 
    insert into mario select NULL,0, 350 ; 
    insert into mario select NULL,660, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,520, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,580, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,480, 80 ; 
    insert into mario select NULL,0, 500 ; 
    insert into mario select NULL,510, 100 ; 
    insert into mario select NULL,0, 450 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 400 ; 
    insert into mario select NULL,320, 100 ; 
    insert into mario select NULL,0, 500 ; 
    insert into mario select NULL,440, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,480, 80 ; 
    insert into mario select NULL,0, 330 ; 
    insert into mario select NULL,450, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 200 ; 
    insert into mario select NULL,660, 80 ; 
    insert into mario select NULL,0, 200 ; 
    insert into mario select NULL,760, 50 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,860, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,700, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,760, 50 ; 
    insert into mario select NULL,0, 350 ; 
    insert into mario select NULL,660, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,520, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,580, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,480, 80 ; 
    insert into mario select NULL,0, 500 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,760, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,720, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,680, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,620, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,650, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,570, 100 ; 
    insert into mario select NULL,0, 220 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,760, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,720, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,680, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,620, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,650, 200 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,1020, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,1020, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,1020, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,760, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,720, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,680, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,620, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,650, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,570, 100 ; 
    insert into mario select NULL,0, 420 ; 
    insert into mario select NULL,585, 100 ; 
    insert into mario select NULL,0, 450 ; 
    insert into mario select NULL,550, 100 ; 
    insert into mario select NULL,0, 420 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 360 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,760, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,720, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,680, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,620, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,650, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,570, 100 ; 
    insert into mario select NULL,0, 220 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,760, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,720, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,680, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,620, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,650, 200 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,1020, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,1020, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,1020, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,760, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,720, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,680, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,620, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,650, 150 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,430, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,570, 100 ; 
    insert into mario select NULL,0, 420 ; 
    insert into mario select NULL,585, 100 ; 
    insert into mario select NULL,0, 450 ; 
    insert into mario select NULL,550, 100 ; 
    insert into mario select NULL,0, 420 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 360 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 60 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 60 ; 
    insert into mario select NULL,0, 350 ; 
    insert into mario select NULL,500, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,580, 80 ; 
    insert into mario select NULL,0, 350 ; 
    insert into mario select NULL,660, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,430, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,380, 80 ; 
    insert into mario select NULL,0, 600 ; 
    insert into mario select NULL,500, 60 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 60 ; 
    insert into mario select NULL,0, 350 ; 
    insert into mario select NULL,500, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,580, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,660, 80 ; 
    insert into mario select NULL,0, 550 ; 
    insert into mario select NULL,870, 80 ; 
    insert into mario select NULL,0, 325 ; 
    insert into mario select NULL,760, 80 ; 
    insert into mario select NULL,0, 600 ; 
    insert into mario select NULL,500, 60 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,500, 60 ; 
    insert into mario select NULL,0, 350 ; 
    insert into mario select NULL,500, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,580, 80 ; 
    insert into mario select NULL,0, 350 ; 
    insert into mario select NULL,660, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,500, 80 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,430, 80 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,380, 80 ; 
    insert into mario select NULL,0, 600 ; 
    insert into mario select NULL,660, 100 ; 
    insert into mario select NULL,0, 150 ; 
    insert into mario select NULL,660, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,660, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,510, 100 ; 
    insert into mario select NULL,0, 100 ; 
    insert into mario select NULL,660, 100 ; 
    insert into mario select NULL,0, 300 ; 
    insert into mario select NULL,770, 100 ; 
    insert into mario select NULL,0, 550 ; 
    insert into mario select NULL,380, 100 ; 
    insert into mario select NULL,0, 575 ; 

    select 'const unsigned char marioDuration[] = { ';

    select concat(dur, ', ') 
    from (
        select id, 
          round(dur/3,0) as dur
        from mario
    ) SUB
    order by id asc
    ;

    select '};';

    select 'const unsigned char marioOfftime[] = { ';

    select concat(offTime, ', ') 
    from (
        select id, 
          round(ifnull((freq/8),0), 0) as offTime
        from mario
    ) SUB
    order by id asc
    ;

    select '};';
    select '#define marioSamples sizeof(marioDuration)';
END
