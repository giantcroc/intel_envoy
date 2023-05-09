#include "source/extensions/compression/zstd/compressor/config.h"

namespace Envoy {
namespace Extensions {
namespace Compression {
namespace Zstd {
namespace Compressor {

ZstdCompressorFactory::ZstdCompressorFactory(
    const envoy::extensions::compression::zstd::compressor::v3::Zstd& zstd,
    Event::Dispatcher& dispatcher, Api::Api& api, ThreadLocal::SlotAllocator& tls,
    Server::Configuration::FactoryContext& context)
    : compression_level_(
          PROTOBUF_GET_WRAPPED_OR_DEFAULT(zstd, compression_level, ZSTD_CLEVEL_DEFAULT)),
      enable_checksum_(zstd.enable_checksum()), strategy_(zstd.strategy()),
      chunk_size_(PROTOBUF_GET_WRAPPED_OR_DEFAULT(zstd, chunk_size, ZSTD_CStreamOutSize())),
      enable_qat_zstd_(zstd.enable_qat_zstd()),
      qat_zstd_fallback_threshold_(PROTOBUF_GET_WRAPPED_OR_DEFAULT(
          zstd, qat_zstd_fallback_threshold, DefaultQatZstdFallbackThreshold)),
      context_(context) {
  if (zstd.has_dictionary()) {
    Protobuf::RepeatedPtrField<envoy::config::core::v3::DataSource> dictionaries;
    dictionaries.Add()->CopyFrom(zstd.dictionary());
    cdict_manager_ = std::make_unique<ZstdCDictManager>(
        dictionaries, dispatcher, api, tls, true,
        [this](const void* dict_buffer, size_t dict_size) -> ZSTD_CDict* {
          return ZSTD_createCDict(dict_buffer, dict_size, compression_level_);
        });
  }
}

Envoy::Compression::Compressor::CompressorPtr ZstdCompressorFactory::createCompressor() {
  return std::make_unique<ZstdCompressorImpl>(compression_level_, enable_checksum_, strategy_,
                                              cdict_manager_, chunk_size_, enable_qat_zstd_,
                                              qat_zstd_fallback_threshold_, context_);
}

Envoy::Compression::Compressor::CompressorFactoryPtr
ZstdCompressorLibraryFactory::createCompressorFactoryFromProtoTyped(
    const envoy::extensions::compression::zstd::compressor::v3::Zstd& proto_config,
    Server::Configuration::FactoryContext& context) {
  return std::make_unique<ZstdCompressorFactory>(proto_config, context.mainThreadDispatcher(),
                                                 context.api(), context.threadLocal(), context);
}

/**
 * Static registration for the zstd compressor library. @see NamedCompressorLibraryConfigFactory.
 */
REGISTER_FACTORY(ZstdCompressorLibraryFactory,
                 Envoy::Compression::Compressor::NamedCompressorLibraryConfigFactory);

} // namespace Compressor
} // namespace Zstd
} // namespace Compression
} // namespace Extensions
} // namespace Envoy
