#pragma once

extern "C"
{
#include "cq_stdlib.h"
};

namespace cqstd
{
	template <typename ValueType>
	class vector
	{
	private:
		size_t m_size;
		size_t m_reservedSize;
		ValueType* m_buffer;

	public:
		typedef ValueType* iterator;
		typedef const ValueType* const_iterator;

		vector() {
			m_reservedSize = 0;
			m_size = 0;
			m_buffer = NULL;
		}

		void copy(const vector& r)
		{
			resize(r.m_size);
			if (r.m_buffer != NULL)
				memcpy(m_buffer, r.m_buffer, m_size * sizeof(ValueType));
		}

		~vector() {
			free(m_buffer);
		}

		void swap(vector& r)
		{
			cq_swap(size_t, m_size, r.m_size);
			cq_swap(size_t, m_reservedSize, r.m_reservedSize);
			cq_swap(ValueType*, m_buffer, r.m_buffer);
		}

		vector(const vector& r)
		{
			m_reservedSize = 0;
			m_size = 0;
			m_buffer = NULL;

			resize(r.size());
			memcpy(m_buffer, r.m_buffer, sizeof(ValueType) * r.m_size);
		}

		size_t size() const {
			return m_size;
		}

		bool empty() const {
			return m_size == 0;
		}

		iterator begin() const {
			return m_buffer;
		}

		iterator end() const {
			return m_buffer + m_size;
		}

		ValueType& front() {
			return m_buffer[0];
		}

		ValueType& back() {
			return m_buffer[m_size - 1];
		}

		void pop_back() {
			CQ_ASSERT(m_size != 0); m_size--;
		}

		ValueType& operator[] (size_t i) {
			return m_buffer[i];
		}

		const ValueType& operator[] (size_t i) const {
			return m_buffer[i];
		}

		ValueType& at(size_t i) {
			return m_buffer[i];
		}

		ValueType* atptr(size_t i) {
			return m_buffer + i;
		}

		void clear() {
			m_size = 0;
		}

		void push_back(const ValueType& obj) {
			if (m_size + 1 > m_reservedSize)
				reserve(m_size + 1);
			if (m_reservedSize > m_size)
			{
				m_buffer[m_size] = obj;
				m_size++;
			}
		}

		void resize(size_t size) {
			reserve(size);
			if (m_reservedSize >= size)
			{
				m_size = size;
			}
		}

		void reserve(size_t newSize) {
			size_t index;
			size_t mask;

			if (newSize <= m_reservedSize)
				return;

			mask = newSize << 1;
			if (cq_bitScanReverse(&index, mask))
				newSize = (size_t)(1) << index;
			else
				return;

			m_buffer = (ValueType*)realloc(m_buffer, newSize * sizeof(ValueType));
			if (m_buffer == NULL)
			{
				CQ_TRACE("vector::reserve, realloc returns NULL, size", newSize * sizeof(ValueType));
				return;
			}
			m_reservedSize = newSize;
		}

		iterator erase(iterator iter) {
			size_t remainBytes;
			CQ_ASSERT(iter >= m_buffer && m_buffer + m_size > iter);
			remainBytes = (m_size - 1 - (iter - m_buffer)) * sizeof(ValueType);
			memmove(iter, iter + 1, remainBytes);
			m_size--;
			return iter;
		}

		iterator erase(iterator s, iterator e) {
			size_t remainBytes;
			CQ_ASSERT(m_buffer <= s && s <= e && e < m_buffer + m_size);
			remainBytes = (m_buffer + m_size - e) * sizeof(ValueType);
			memmove(s, e, remainBytes);
			m_size -= e - s;
			return s;
		}

		void insert(iterator loc, const ValueType& obj) {
			size_t _where = loc - m_buffer;
			if (m_size + 1 > m_reservedSize)
				reserve(m_size + 1);
			if (m_reservedSize > m_size)
			{
				loc = m_buffer + _where;
				memmove(loc + 1, loc, (m_size - (loc - m_buffer)) * sizeof(ValueType));
				*loc = obj;
				m_size++;
			}
		}

		void insert(iterator loc, iterator s, iterator e) {
			size_t _where = loc - m_buffer;
			size_t size = e - s;
			if (m_size + size > m_reservedSize)
				reserve(m_size + size);
			if (m_reservedSize > m_size)
			{
				loc = m_buffer + _where;
				memmove(loc + size, loc, (m_size - (loc - m_buffer)) * sizeof(ValueType));
				memcpy(loc, s, sizeof(ValueType) * size);
				m_size += size;
			}
		}

		void assign(iterator s, iterator e) {
			clear();
			insert(end(), s, e);
		}

	private:
		void operator = (const vector& r);
	};
}
