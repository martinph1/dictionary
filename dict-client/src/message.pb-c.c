/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C_NO_DEPRECATED
#define PROTOBUF_C_NO_DEPRECATED
#endif

#include "message.pb-c.h"
void   server_command__init
                     (ServerCommand         *message)
{
  static ServerCommand init_value = SERVER_COMMAND__INIT;
  *message = init_value;
}
size_t server_command__get_packed_size
                     (const ServerCommand *message)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &server_command__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t server_command__pack
                     (const ServerCommand *message,
                      uint8_t       *out)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &server_command__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t server_command__pack_to_buffer
                     (const ServerCommand *message,
                      ProtobufCBuffer *buffer)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &server_command__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
ServerCommand *
       server_command__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (ServerCommand *)
     protobuf_c_message_unpack (&server_command__descriptor,
                                allocator, len, data);
}
void   server_command__free_unpacked
                     (ServerCommand *message,
                      ProtobufCAllocator *allocator)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &server_command__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
const ProtobufCEnumValue server_command__command__enum_values_by_number[3] =
{
  { "NOP", "SERVER_COMMAND__COMMAND__NOP", 0 },
  { "START", "SERVER_COMMAND__COMMAND__START", 1 },
  { "DISCONNECT", "SERVER_COMMAND__COMMAND__DISCONNECT", 20 },
};
static const ProtobufCIntRange server_command__command__value_ranges[] = {
{0, 0},{20, 2},{0, 3}
};
const ProtobufCEnumValueIndex server_command__command__enum_values_by_name[3] =
{
  { "DISCONNECT", 2 },
  { "NOP", 0 },
  { "START", 1 },
};
const ProtobufCEnumDescriptor server_command__command__descriptor =
{
  PROTOBUF_C_ENUM_DESCRIPTOR_MAGIC,
  "server_command.command",
  "command",
  "ServerCommand__Command",
  "",
  3,
  server_command__command__enum_values_by_number,
  3,
  server_command__command__enum_values_by_name,
  2,
  server_command__command__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor server_command__field_descriptors[1] =
{
  {
    "command_id",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    PROTOBUF_C_OFFSETOF(ServerCommand, command_id),
    &server_command__command__descriptor,
    NULL,
    0,            /* packed */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned server_command__field_indices_by_name[] = {
  0,   /* field[0] = command_id */
};
static const ProtobufCIntRange server_command__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor server_command__descriptor =
{
  PROTOBUF_C_MESSAGE_DESCRIPTOR_MAGIC,
  "server_command",
  "ServerCommand",
  "ServerCommand",
  "",
  sizeof(ServerCommand),
  1,
  server_command__field_descriptors,
  server_command__field_indices_by_name,
  1,  server_command__number_ranges,
  (ProtobufCMessageInit) server_command__init,
  NULL,NULL,NULL    /* reserved[123] */
};
