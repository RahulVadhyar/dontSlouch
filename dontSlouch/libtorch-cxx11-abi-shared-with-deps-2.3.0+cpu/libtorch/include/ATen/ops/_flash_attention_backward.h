#pragma once

// @generated by torchgen/gen.py from Function.h

#include <ATen/Context.h>
#include <ATen/DeviceGuard.h>
#include <ATen/TensorUtils.h>
#include <ATen/TracerMode.h>
#include <ATen/core/Generator.h>
#include <ATen/core/Reduction.h>
#include <ATen/core/Tensor.h>
#include <c10/core/Scalar.h>
#include <c10/core/Storage.h>
#include <c10/core/TensorOptions.h>
#include <c10/util/Deprecated.h>
#include <c10/util/Optional.h>



#include <ATen/ops/_flash_attention_backward_ops.h>

namespace at {


// aten::_flash_attention_backward(Tensor grad_out, Tensor query, Tensor key, Tensor value, Tensor out, Tensor logsumexp, Tensor cum_seq_q, Tensor cum_seq_k, SymInt max_q, SymInt max_k, float dropout_p, bool is_causal, Tensor philox_seed, Tensor philox_offset, *, float? scale=None) -> (Tensor, Tensor, Tensor)
inline ::std::tuple<at::Tensor,at::Tensor,at::Tensor> _flash_attention_backward(const at::Tensor & grad_out, const at::Tensor & query, const at::Tensor & key, const at::Tensor & value, const at::Tensor & out, const at::Tensor & logsumexp, const at::Tensor & cum_seq_q, const at::Tensor & cum_seq_k, int64_t max_q, int64_t max_k, double dropout_p, bool is_causal, const at::Tensor & philox_seed, const at::Tensor & philox_offset, c10::optional<double> scale=c10::nullopt) {
    return at::_ops::_flash_attention_backward::call(grad_out, query, key, value, out, logsumexp, cum_seq_q, cum_seq_k, max_q, max_k, dropout_p, is_causal, philox_seed, philox_offset, scale);
}
namespace symint {
  template <typename T, typename = std::enable_if_t<std::is_same<T, int64_t>::value>>
  ::std::tuple<at::Tensor,at::Tensor,at::Tensor> _flash_attention_backward(const at::Tensor & grad_out, const at::Tensor & query, const at::Tensor & key, const at::Tensor & value, const at::Tensor & out, const at::Tensor & logsumexp, const at::Tensor & cum_seq_q, const at::Tensor & cum_seq_k, int64_t max_q, int64_t max_k, double dropout_p, bool is_causal, const at::Tensor & philox_seed, const at::Tensor & philox_offset, c10::optional<double> scale=c10::nullopt) {
    return at::_ops::_flash_attention_backward::call(grad_out, query, key, value, out, logsumexp, cum_seq_q, cum_seq_k, max_q, max_k, dropout_p, is_causal, philox_seed, philox_offset, scale);
  }
}

// aten::_flash_attention_backward(Tensor grad_out, Tensor query, Tensor key, Tensor value, Tensor out, Tensor logsumexp, Tensor cum_seq_q, Tensor cum_seq_k, SymInt max_q, SymInt max_k, float dropout_p, bool is_causal, Tensor philox_seed, Tensor philox_offset, *, float? scale=None) -> (Tensor, Tensor, Tensor)
inline ::std::tuple<at::Tensor,at::Tensor,at::Tensor> _flash_attention_backward_symint(const at::Tensor & grad_out, const at::Tensor & query, const at::Tensor & key, const at::Tensor & value, const at::Tensor & out, const at::Tensor & logsumexp, const at::Tensor & cum_seq_q, const at::Tensor & cum_seq_k, c10::SymInt max_q, c10::SymInt max_k, double dropout_p, bool is_causal, const at::Tensor & philox_seed, const at::Tensor & philox_offset, c10::optional<double> scale=c10::nullopt) {
    return at::_ops::_flash_attention_backward::call(grad_out, query, key, value, out, logsumexp, cum_seq_q, cum_seq_k, max_q, max_k, dropout_p, is_causal, philox_seed, philox_offset, scale);
}
namespace symint {
  template <typename T, typename = std::enable_if_t<std::is_same<T, c10::SymInt>::value>>
  ::std::tuple<at::Tensor,at::Tensor,at::Tensor> _flash_attention_backward(const at::Tensor & grad_out, const at::Tensor & query, const at::Tensor & key, const at::Tensor & value, const at::Tensor & out, const at::Tensor & logsumexp, const at::Tensor & cum_seq_q, const at::Tensor & cum_seq_k, c10::SymInt max_q, c10::SymInt max_k, double dropout_p, bool is_causal, const at::Tensor & philox_seed, const at::Tensor & philox_offset, c10::optional<double> scale=c10::nullopt) {
    return at::_ops::_flash_attention_backward::call(grad_out, query, key, value, out, logsumexp, cum_seq_q, cum_seq_k, max_q, max_k, dropout_p, is_causal, philox_seed, philox_offset, scale);
  }
}

}