//  Copyright (c) 2016-2017 The KID Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "packet_process_assist.h"


namespace net {

bool checksum(const char* src_data, uint16&  check_ret) {
  bool ret = true;

  do {
    const int16_t* opt_data = reinterpret_cast<const int16_t*>(src_data);
    if (opt_data == NULL || src_data == NULL) {
        ret = false;
        /*TODO*/
        // warn_log();
        break;
    }
    int32_t accu_sum = 0;
    int32_t data_len = strlen(src_data);

    while (data_len > 1) {
        accu_sum += *(opt_data);
        opt_data++;
        data_len = data_len - 2;

        if (accu_sum & 0x80000000) {
            accu_sum = (accu_sum >> 16) + (accu_sum & 0xFFFF);
        }
    }

    if (data_len == 1) {
        accu_sum += *(reinterpret_cast<const int8_t*>(opt_data));
    }

    while (accu_sum >> 16) {
        accu_sum = (accu_sum >> 16) + (accu_sum & 0xFFFF);
    }

    //check_ret = (accu_sum == 0xFFFF)?~accu_sum:accu_sum;
    check_ret = 0xFFFF & accu_sum;
  }while(0);

  return ret;
}

//加密 key
unsigned int *key = (unsigned int *)"1234567890abcdek";  // NOLINT //key

// jiami
void encrypt(unsigned int *v1, unsigned int *v2, unsigned int *k, int rounds) {
    unsigned int y = *v1, z = *v2, sum = 0, i = 0;
    unsigned int delta = 0x9e3779b9;
    int a = k[0], b = k[1], c = k[2], d = k[3];
    for (i=0; i < rounds; i++) {
      sum += delta;
      y += ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
      z += ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
    }
    *v1 = y;
    *v2 = z;
}

// jiemi
void decrypt(unsigned int *v1, unsigned int *v2, unsigned int *k, int rounds) {
    unsigned int y = *v1, z = *v2, i = 0, sum;
    if (rounds == 32)
        sum = 0xc6ef3720;
    else
        sum = 0xe3779b90;
        // sum = 0x9e3779b9;
    unsigned int delta = 0x9e3779b9;
    int a = k[0], b = k[1], c = k[2], d = k[3];
    for (i = 0; i < rounds; i++) {
      z -= ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
      y -= ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
      sum -= delta;
    }
    *v1 = y;
    *v2 = z;
}

// 16 or 32 rounds jiami
char* str_en_8byte(char *clearmsg, int len, int rounds, int& len1) {
    //int len = strlen(clearmsg);
    //int n = len/8;
    //int len1 = (n+1)*8;//传入的大小已经补全8 的倍数
    len1 = len;
    int  i;
    char *en = (char*)malloc(len1*sizeof(char) + 1);  // NOLINT
    //memcpy(en, clearmsg, strlen(clearmsg));
    memcpy(en, clearmsg, len);
    //for (i = len; i < len1; ++i)
    //    *(en+i) = '\0';
    unsigned int *y1, *y2;
    for (i = 0; i < len1; i += 8) {
        y1 = (unsigned int *)&en[i];  // NOLINT
        y2 = (unsigned int *)&en[i+4];  // NOLINT
        encrypt(y1, y2, key, rounds);
    }
    en[len1] = '\0';
    return en;
}


// 16 or 32 rounds jiemi
char* str_de_8byte(char *encryptedmsg, int len, int rounds) {
    int i;
    //int len = strlen(encryptedmsg);
    char *de = (char*)malloc(len*sizeof(char) + 1);  // NOLINT
    //memcpy(de, encryptedmsg, sizeof(encryptedmsg));
    memcpy(de, encryptedmsg, len);
    unsigned int *y1, *y2;
    for (i = 0; i < len; i += 8) {
        y1 = (unsigned int *)&de[i];  // NOLINT
        y2 = (unsigned int *)&de[i+4];  // NOLINT
        decrypt(y1, y2, key, rounds);
    }
    *(de+len) = '\0';
    return de;
}

std::string str_en(std::string a, int rounds) {
    char destCh[100];
    memset(destCh, '\0', sizeof(destCh));
    int start = 0, end = 0, num = 0, i;
    std::string temp[100];
    for (i=0; i<a.length(); i+=8) {
        if (i+8 > a.length()) {
            temp[num] = a.substr(i, a.length()-i+1);
            num++;
        } else {
            temp[num] = a.substr(i, 8);
            num++;
        }
    }
    int j, len;
    char* tempCh = NULL;
    for (j=0; j<num; j++)
    {
        tempCh = str_en_8byte((char *)temp[j].c_str(), temp[j].length(), 16, len);
        strcat(destCh, tempCh);
    }
    std::string strEn = destCh;
    return strEn;
}

std::string str_de(std::string strEn, int rounds) {
    char* tempCh = NULL;
    int num = 0, i, j;
    char destCh[100];
    memset(destCh, '\0', sizeof(destCh));
    std::string tempDe[100];
    for (i=0; i<strEn.length(); i+=8) {
        if (i+8 > strEn.length()) {
            tempDe[num] = strEn.substr(i, strEn.length()-i+1);
            num++;
        } else {
            tempDe[num] = strEn.substr(i, 8);
            num++;
        }
    }
    for (j=0; j<num; j++)
    {
        tempCh = str_de_8byte((char *)tempDe[j].c_str(), tempDe[j].length(), 16);
        strcat(destCh, tempCh);
    }
    std::string strDe = destCh;
    return strDe;
}



MZip::MZip(void)
    : zlibStream_(NULL)
    , ulTotalZipLen_(0)
    , bCompress_(true)
{
}

MZip::~MZip(void) {
    if(zlibStream_) {
        deflateEnd(zlibStream_);
        delete zlibStream_;
        zlibStream_ = NULL;
    }
}

unsigned long MZip::ZipData(const unsigned char* pUnZipData, unsigned long ulUnZipLen, 
                            unsigned char** ppZipData) {
    if(pUnZipData == NULL || ulUnZipLen == 0) {
        return 0;
    }

    unsigned long ulZipLen = 0;
    do{
        if(bCompress_ == false) {
            break;
        }
        if(ulUnZipLen < ENCODE_ZLIB_MIN_COMP_SIZE) {
            break;
        }
        *ppZipData = new unsigned char[ulUnZipLen + 300];
        if(*ppZipData == NULL) {
            break;
        }
        ulZipLen = ulUnZipLen + 300;
        int Ret = Zip(pUnZipData,ulUnZipLen,*ppZipData,ulZipLen);
        if(Ret != Z_OK) {
            delete []*ppZipData;
            ulZipLen = 0;
            break;
        }
        return ulZipLen;

    }while(0);

    *ppZipData = new unsigned char[ulUnZipLen];
    if(*ppZipData) {
        *(unsigned char*)*ppZipData = ZIP_TYPE_NONE;
        memcpy(*ppZipData,pUnZipData,ulUnZipLen);
        ulZipLen = ulUnZipLen;
    }
    return ulZipLen;
}

int MZip::Zip(const unsigned char* pUnZipData, unsigned long ulUnZipLen, 
                unsigned char* pZipData, unsigned long& ulZipLen) {
    int err = Z_DATA_ERROR;
    if (zlibStream_ == NULL) {
        zlibStream_ = new z_stream;
        // Initialise stream values
        zlibStream_->zalloc = (alloc_func)0;
        zlibStream_->zfree = (free_func)0;
        zlibStream_->opaque = (voidpf)0;

        // Initialise the z_stream
        err = deflateInit(zlibStream_,9);//9:Z_BEST_COMPRESSION
        if (err != Z_OK) {
            return err;
        }
        ulTotalZipLen_ = 0;
    }
    // Use whatever input is provided
    zlibStream_->next_in = const_cast<Bytef*>(pUnZipData);
    zlibStream_->avail_in = ulUnZipLen;
    zlibStream_->next_out = pZipData;
    zlibStream_->avail_out = ulZipLen;
    // Try to unzip the data
    err = deflate(zlibStream_, Z_SYNC_FLUSH);    
    if ((err == Z_OK) && (zlibStream_->avail_in == 0)) {
        // All available input has been processed, everything ok.
        // Set the size to the amount unzipped in this call (including all recursive calls)
        ulZipLen = (zlibStream_->total_out - ulTotalZipLen_);
        ulTotalZipLen_ = zlibStream_->total_out;
    }

    if (err != Z_OK)
        ulZipLen = 0;
    return err;
}

void MZip::EnableCompress(bool bEnable) {
    bCompress_ = bEnable;
}



#define MAX_BLOCKSIZE               184320

MUnZip::MUnZip(void)
    : zlibStream_(NULL)
    , unZipLen_(0)
{
}

MUnZip::~MUnZip(void)
{
    if(zlibStream_) {
        inflateEnd(zlibStream_);
        delete zlibStream_;
        zlibStream_ = NULL;
    }
}

unsigned long MUnZip::UnZipData(const unsigned char* pZipData, unsigned long ulZipLen, 
                                unsigned char** ppUnZipData) {
    if(pZipData == NULL || ulZipLen == 0) {
        return 0;
    }
    unsigned long ulUnZipLen = 0;
    unsigned char ucType = *(unsigned char*)pZipData;

    ulUnZipLen = ulZipLen * 2;
    if(ulUnZipLen > MAX_BLOCKSIZE + 300) {
        ulUnZipLen = MAX_BLOCKSIZE + 300;
    }
    *ppUnZipData = new unsigned char[ulUnZipLen];
    if(*ppUnZipData) {
        int Ret = UnZip(pZipData,ulZipLen,ppUnZipData,&ulUnZipLen);
        if(Ret != Z_OK || ulUnZipLen == 0) {
            delete []*ppUnZipData;
            *ppUnZipData = NULL;
            ulUnZipLen = 0;
        }
    }
    return ulUnZipLen;
}

int MUnZip::UnZip(const unsigned char* pZipData, unsigned long ulZipLen, 
                    unsigned char** ppUnZipData, unsigned long* ulUnZipLen, int iRecursion) {
    int err = Z_DATA_ERROR;
    if (zlibStream_ == NULL) {
        zlibStream_ = new z_stream;
        // Initialise stream values
        zlibStream_->zalloc = (alloc_func)0;
        zlibStream_->zfree = (free_func)0;
        zlibStream_->opaque = (voidpf)0;

        // Set output data streams, do this here to avoid overwriting on recursive calls
        zlibStream_->next_out = (*ppUnZipData);
        zlibStream_->avail_out = (*ulUnZipLen);

        // Initialise the z_stream
        err = inflateInit(zlibStream_);
        if (err != Z_OK)
        {
            return err;
        }
        unZipLen_ = 0;
    }
    // Use whatever input is provided
    zlibStream_->next_in     = const_cast<Bytef*>(pZipData);
    zlibStream_->avail_in = ulZipLen;

    // Only set the output if not being called recursively
    if (iRecursion == 0) {
        zlibStream_->next_out = (*ppUnZipData);
        zlibStream_->avail_out = (*ulUnZipLen);
    }

    // Try to unzip the data
    err = inflate(zlibStream_, Z_SYNC_FLUSH);    
    if ((err == Z_OK) && (zlibStream_->avail_out == 0) && (zlibStream_->avail_in != 0)) {
        // Output array was not big enough, call recursively until there is enough space
        // What size should we try next
        unsigned long newLength = (*ulUnZipLen) *= 2;
        if (newLength == 0)
            newLength = ulZipLen * 2;

        // Copy any data that was successfully unzipped to new array
        BYTE *temp = new BYTE[newLength];
        memcpy(temp, (*ppUnZipData), (zlibStream_->total_out - unZipLen_));
        delete[] (*ppUnZipData);
        (*ppUnZipData) = temp;
        (*ulUnZipLen) = newLength;

        // Position stream output to correct place in new array
        zlibStream_->next_out = (*ppUnZipData) + (zlibStream_->total_out - unZipLen_);
        zlibStream_->avail_out = (*ulUnZipLen) - (zlibStream_->total_out - unZipLen_);

        // Try again
        err = UnZip(zlibStream_->next_in, zlibStream_->avail_in, ppUnZipData, ulUnZipLen, iRecursion + 1);
    }
    else if ((err == Z_OK) && (zlibStream_->avail_in == 0)) {
        // All available input has been processed, everything ok.
        // Set the size to the amount unzipped in this call (including all recursive calls)
        (*ulUnZipLen) = (zlibStream_->total_out - unZipLen_);
        unZipLen_ = zlibStream_->total_out;
    }

    if (err != Z_OK)
        (*ulUnZipLen) = 0;
    return err;
}


}  //  namespace net
