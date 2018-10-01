#!/bin/tcsh -fv

# http://en.wikipedia.org/wiki/Piano_key_frequencies

cat << END | mysql -sr -upaul -ppaul paul
    drop table if exists piano
    ;

    create table piano (
       id int not NULL,
       name varchar(4),
       freq float
    ) engine=myisam;

    insert into piano select '88', 'c', '4186.01';
    insert into piano select '87', 'b', '3951.07';
    insert into piano select '86', 'a#', '3729.31';
    insert into piano select '85', 'a', '3520.00';
    insert into piano select '84', 'g#', '3322.44';
    insert into piano select '83', 'g', '3135.96';
    insert into piano select '82', 'f#', '2959.96';
    insert into piano select '81', 'f', '2793.83';
    insert into piano select '80', 'e', '2637.02';
    insert into piano select '79', 'd#', '2489.02';
    insert into piano select '78', 'd', '2349.32';
    insert into piano select '77', 'c#', '2217.46';
    insert into piano select '76', 'c', '2093.00';
    insert into piano select '75', 'b', '1975.53';
    insert into piano select '74', 'a#', '1864.66';
    insert into piano select '73', 'a', '1760.00';
    insert into piano select '72', 'g#', '1661.22';
    insert into piano select '71', 'g', '1567.98';
    insert into piano select '70', 'f#', '1479.98';
    insert into piano select '69', 'f', '1396.91';
    insert into piano select '68', 'e', '1318.51';
    insert into piano select '67', 'd#', '1244.51';
    insert into piano select '66', 'd', '1174.66';
    insert into piano select '65', 'c#', '1108.73';
    insert into piano select '64', 'c', '1046.50';
    insert into piano select '63', 'b', '987.767';
    insert into piano select '62', 'a#', '932.328';
    insert into piano select '61', 'a', '880.000';
    insert into piano select '60', 'g#', '830.609';
    insert into piano select '59', 'g', '783.991';
    insert into piano select '58', 'f#', '739.989';
    insert into piano select '57', 'f', '698.456';
    insert into piano select '56', 'e', '659.255';
    insert into piano select '55', 'd#', '622.254';
    insert into piano select '54', 'd', '587.330';
    insert into piano select '53', 'c#', '554.365';
    insert into piano select '52', 'c', '523.251';
    insert into piano select '51', 'b', '493.883';
    insert into piano select '50', 'a#', '466.164';
    insert into piano select '49', 'a', '440.000';
    insert into piano select '48', 'g#', '415.305';
    insert into piano select '47', 'g', '391.995';
    insert into piano select '46', 'f#', '369.994';
    insert into piano select '45', 'f', '349.228';
    insert into piano select '44', 'e', '329.628';
    insert into piano select '43', 'd#', '311.127';
    insert into piano select '42', 'd', '293.665';
    insert into piano select '41', 'c#', '277.183';
    insert into piano select '40', 'c', '261.626';
    insert into piano select '39', 'b', '246.942';
    insert into piano select '38', 'a#', '233.082';
    insert into piano select '37', 'a', '220.000';
    insert into piano select '36', 'g#', '207.652';
    insert into piano select '35', 'g', '195.998';
    insert into piano select '34', 'f#', '184.997';
    insert into piano select '33', 'f', '174.614';
    insert into piano select '32', 'e', '164.814';
    insert into piano select '31', 'd#', '155.563';
    insert into piano select '30', 'd', '146.832';
    insert into piano select '29', 'c#', '138.591';
    insert into piano select '28', 'c', '130.813';
    insert into piano select '27', 'B', '123.471';
    insert into piano select '26', 'A#', '116.541';
    insert into piano select '25', 'A', '110.000';
    insert into piano select '24', 'G#', '103.826';
    insert into piano select '23', 'G', '97.9989';
    insert into piano select '22', 'F#', '92.4986';
    insert into piano select '21', 'F', '87.3071';
    insert into piano select '20', 'E', '82.4069';
    insert into piano select '19', 'D#', '77.7817';
    insert into piano select '18', 'D', '73.4162';
    insert into piano select '17', 'C#', '69.2957';
    insert into piano select '16', 'C', '65.4064';
    insert into piano select '15', 'B', '61.7354';
    insert into piano select '14', 'A#', '58.2705';
    insert into piano select '13', 'A', '55.0000';
    insert into piano select '12', 'G#', '51.9131';
    insert into piano select '11', 'G', '48.9994';
    insert into piano select '10', 'F#', '46.2493';
    insert into piano select '9', 'F', '43.6535';
    insert into piano select '8', 'E', '41.2034';
    insert into piano select '7', 'D#', '38.8909';
    insert into piano select '6', 'D', '36.7081';
    insert into piano select '5', 'C#', '34.6478';
    insert into piano select '4', 'C', '32.7032';
    insert into piano select '3', 'B', '30.8677';
    insert into piano select '2', 'A#', '29.1352';
    insert into piano select '1', 'A', '27.5000';

    select 'struct pianoNotes { ';
    select '   char name[2] ; ';
    select '   unsigned short offTime; ';
    select '} pianoNotes ; ';

    select 'struct pianoNotes piano[] = { ';

    select concat('{ "', name, '",', offTime, ' }, ') 
    from (
        select id, rpad(name,2,'_') as name,
          38000 / freq as f_offTime,
          round(38000 / freq, 0) as offTime
        from piano
    ) SUB
    order by id asc
    ;

    select '};';
END


