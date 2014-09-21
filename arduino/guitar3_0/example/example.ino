#include <SPI.h>
#include <SdFat.h>

// Control Select pin of SD card
const uint8_t SD_CS_PIN = 9;

SdFat sd;
SdBaseFile* dirFile;

// Search for the first file in a directory
//
// parameters:
//   char * lfn : where to store the 'long name' file
//                (must have a dim of at least 131 char)
//   char * sdn : the directory (short name) we want to scan
//
// return:
//   a dir_t object pointing to the founded file

dir_t dirLfnFirst( char * lfn, char * sdn )
{
  sd.chdir( sdn );
  dirFile = sd.vwd();
  dirFile->rewind();
  return dirLfnNext( lfn );
}

// Search for the next file
//
// parameters:
//   char * lfn : where to store the 'long name' file
//                (must have a dim of at least 131 char)
//
// return:
//   a dir_t object pointing to the founded file

dir_t dirLfnNext( char * lfn )
{
  uint8_t offset[] = {1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30};
  dir_t dir;
  uint8_t lfnIn = 130;
  uint8_t i;
  uint8_t ndir;
  uint8_t sum;
  uint8_t test;
  bool haveLong = false;

  while( dirFile->read( &dir, 32 ) == 32 )
  {
    if( DIR_IS_LONG_NAME( &dir ) )
    {
      if( ! haveLong )
      {
        if(( dir.name[0] & 0XE0 ) != 0X40 )
          continue;
        ndir = dir.name[0] & 0X1F;
        test = dir.creationTimeTenths;
        haveLong = true;
        lfnIn = 130;
        lfn[ lfnIn ] = 0;
      }
      else if( dir.name[0] != --ndir || test != dir.creationTimeTenths )
      {
        haveLong = false;
        continue;
      }
      char *p = (char*) & dir;
      if( lfnIn > 0 )
      {
        lfnIn -= 13;
        for( i = 0; i < 13; i++ )
          lfn[lfnIn + i] = p[offset[i]];
      }
    }
    else if( DIR_IS_FILE_OR_SUBDIR( &dir ) 
             && dir.name[0] != DIR_NAME_DELETED 
             && dir.name[0] != DIR_NAME_FREE
             && dir.name[0] != '.' )
    {
      if( haveLong )
      {
        for( sum = i = 0; i < 11; i++ )
           sum = (((sum & 1) << 7) | ((sum & 0xfe) >> 1)) + dir.name[i];
        if( sum != test || ndir != 1 )
        haveLong = false;
      }
      if( haveLong )
      {
        for( i = 0; lfnIn + i <= 130 ; i++ )
          lfn[i] = lfn[lfnIn + i];
        return dir;
      }
      // else if( dir.reservedNT )
      //  return "Reserved NT";
      else
      {
        SdFile::dirName( dir, lfn );
        return dir;  
      }
    }
    else
    {
      if( dir.name[0] == DIR_NAME_FREE )
        break;
      haveLong = false;
    }
  }
  lfn[ 0 ] = 0;
  return dir;
}

// Convert a 'long name' file to a 'short name' file
//
// parameters:
//   char * sn : where to store the 'short name' file
//   char * dn : the directory (short name) where is the file
//   char * ln : the 'long name' file we want to convert
//   size_t lln : the length of the long name file. If 0 (default),
//                calculated with strlen( ln )
//
// return:
//   true, if convertion is done

boolean l2sName( char * sn, char * dn, char * ln, size_t lln = 0 );

boolean l2sName( char * sn, char * dn, char * ln, size_t lln )
{
  char ln0[131];
  ln0[0] = 0;
  dir_t dir = dirLfnFirst( ln0, dn );
  if( lln == 0 )
    lln = strlen( ln );
  while(( strlen( ln0 ) > 0 ) && ( strncmp( ln0, ln, lln ) != 0 ))
    dir = dirLfnNext( ln0 );
  SdFile::dirName( dir, sn );
  return strlen( sn ) != 0;
}

// Convert a 'long name' path to a 'short name' path
//
// parameters:
//   char * snd : where to store the 'short name' path
//   char * lnd : the 'long name' path we want to convert
//   size_t maxsnl : the length of snd string
//
// return:
//   true, if convertion is done

boolean l2sPath( char * snd, char * lnd, size_t maxsnl )
{
  char * dir0 = lnd + 1;
  char * dir1;
  char * snd1;
  
  strcpy( snd, "/" );
  while( strlen( dir0 ) > 1 )
  {
    dir1 = strchr( dir0, '/' );
    if( dir1 == 0 )
      return true;
    if( maxsnl <= strlen( snd ) + 8 )
      return false;
    snd1 = strrchr( snd, 0 );
    // same as:  snd1 = snd + strlen( snd );
    if( ! l2sName( snd1 , snd, dir0, dir1 - dir0 ))
      return false;
    strcat( snd, "/" );
    dir0 = dir1 + 1;
  }
  return true;
}

// Convert a long full name (path+name) to a short full name
//
// parameters:
//   char * sfn : where to store the short full name
//   char * lfn : the long full name we want to convert
//   size_t maxsfn : the length of sfn string
//
// return:
//   true, if convertion is done

boolean l2sFullName( char * sfn, char * lfn, size_t maxsfn )
{
  if( ! l2sPath( sfn, lfn, maxsfn ))
    return false;
  char * sfn1 = strrchr( sfn, 0 );
  char * ln = strrchr( lfn, '/' ) + 1;
  if( ln == 0 )
    return false;
  return l2sName( sfn1, sfn, ln );
}

//
//   DEMONSTRATE THE USE OF PREVIOUS FUNCTIONS
//
//   You must modify content of strings theDir , theLongNameDir , theLongName
//     and theLongFullName according to content of your SD card
//

void setup()
{
  Serial.begin( 9600 );

  // Control Select of SD card of MP3 shield set to high
  pinMode( SD_CS_PIN, OUTPUT );
  digitalWrite( SD_CS_PIN, HIGH );
  if( ! sd.begin( SD_CS_PIN ))
    sd.initErrorHalt();
 
  // Demonsrate use of dirLfnFirst() and dirLfnNext()
  char * theDir = "/THEBEA~1/PLEASE~1/";
  Serial.println( "List all files by their long name in directory" );
  Serial.println( theDir );
  Serial.println( "given by his short name:" );
  Serial.println( );
  char fileName[131];
  dirLfnFirst( fileName, theDir );
  while( strlen( fileName ) > 0 )
  {
    Serial.println( fileName );
    dirLfnNext( fileName );
  }

  // Demonsrate use of l2sPath(), dirLfnFirst() and dirLfnNext()
  // Show use of returned dir object to recover short name
  char * theLongNameDir = "/The Beatles/Please Please Me/";
  Serial.println( ); Serial.println( );
  Serial.println( "List all files by their short and long names in directory" );
  Serial.println( theLongNameDir );
  Serial.println( "given by his long name:" );
  Serial.println( );
  char shortPath[100];
  char shortName[13];
  dir_t dir;
  if( l2sPath( shortPath, theLongNameDir, 100 ))
  {
    dir = dirLfnFirst( fileName, shortPath );
    while( strlen( fileName ) > 0 )
    {
      SdFile::dirName( dir, shortName );
      Serial.print( shortName );
      Serial.print( " ; " );
      Serial.println( fileName );
      dir = dirLfnNext( fileName );
    }
  }
  else
    Serial.println( " not found!" );

  // Demonsrate use of l2sName()
  char shortFileName[13];
  char * theLongName = "07-Please Please Me.mp3";
  Serial.println( ); Serial.println( );
  Serial.println( "The short name of " );
  Serial.println( theLongName );
  Serial.println( "in directory" );
  Serial.println( theDir );
  Serial.println( "is:" );
  Serial.println( );
  if( l2sName( shortFileName, theDir, theLongName ))
    Serial.println( shortFileName );
  else
    Serial.println( "not found :(" );

  // Demonsrate use of l2sFullName()
  char * theLongFullName = "/The Beatles/Please Please Me/11-Do You Want To Know A Secret.mp3";
  Serial.println( ); Serial.println( );
  Serial.println( "The short full name of  " );
  Serial.println( theLongFullName );
  Serial.println( "is:" );
  Serial.println( );
  if( l2sFullName( shortPath, theLongFullName, 100 ))
    Serial.println( shortPath );
  else
    Serial.println( "not found :(" );
}

void loop()
{
}
