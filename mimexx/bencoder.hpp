#ifndef __B_ENCODER_H_
#define __B_ENCODER_H_

namespace encoder
{

  // encode and decode for BASE64
  string encode_base64 (const string& src_str);
  string decode_base64 (const string& src_str);

  // encode and decode for XXencoding
  string encode_xx (const string& src_str);
  string decode_xx (const string& src_str);

  // encode and decode for BinHex
  string encode_binhex (const string& src_str);
  string decode_binhex (const string& src_str);

  // encode and decode for quoted-printable
  string encode_qp (const string& src_str);
  string decode_qp (const string& src_str);

  // encode and decode rot13
  string convert_rot13(const string& src_str);
  string encode_rot13 (const string& src_str);
  string decode_rot13 (const string& src_str);

  // encode and decode hz (hanzi)
  string encode_hz (const string& src_str);
  string decode_hz (const string& src_str);
}

#endif __B_ENCODER_H_
