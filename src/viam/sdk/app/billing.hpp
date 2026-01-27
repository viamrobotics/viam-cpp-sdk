#pragma once

#include <memory>
#include <string>
#include <vector>

#include <viam/api/app/v1/billing.grpc.pb.h>
#include <viam/api/app/v1/billing.pb.h>
#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {

class BillingClient {
   public:
    explicit BillingClient(ViamChannel channel);

    const ViamChannel& channel() const;

    // Detailed breakdown of current month's costs
    //
    // Args:
    //   org_id: The organization ID.
    //
    // Returns:
    //   A GetCurrentMonthUsageResponse.
    viam::app::v1::GetCurrentMonthUsageResponse GetCurrentMonthUsage(const std::string& org_id);

    // Org-level information (like billing email and payment details)
    //
    // Args:
    //   org_id: The organization ID.
    //
    // Returns:
    //   A GetOrgBillingInformationResponse.
    viam::app::v1::GetOrgBillingInformationResponse GetOrgBillingInformation(const std::string& org_id);

    // Total outstanding balance and previous invoices
    //
    // Args:
    //   org_id: The organization ID.
    //
    // Returns:
    //   A GetInvoicesSummaryResponse.
    viam::app::v1::GetInvoicesSummaryResponse GetInvoicesSummary(const std::string& org_id);

    // Download a PDF invoice
    //
    // Args:
    //   id: The invoice ID.
    //   org_id: The organization ID.
    //
    // Returns:
    //   A vector of bytes representing the PDF content.
    std::vector<uint8_t> GetInvoicePdf(const std::string& id, const std::string& org_id);

    // Send an email with a prompt to the user's org's billing page.
    //
    // Args:
    //   customer_org_id: The customer organization ID.
    //   billing_owner_org_id: The billing owner organization ID.
    void SendPaymentRequiredEmail(const std::string& customer_org_id, const std::string& billing_owner_org_id);

    // Get available billing tiers that can be assigned to organizations
    //
    // Returns:
    //   A GetAvailableBillingTiersResponse.
    viam::app::v1::GetAvailableBillingTiersResponse GetAvailableBillingTiers();

    // Update an organization's billing tier
    //
    // Args:
    //   organization_id: The organization ID.
    //   billing_tier: The new billing tier.
    void UpdateOrganizationBillingTier(const std::string& organization_id, const std::string& billing_tier);

    // Charge an organization on the spot
    //
    // Args:
    //   org_id_to_charge: The organization ID to charge.
    //   subtotal: The subtotal amount to charge.
    //   tax: The tax amount to charge.
    //   description: An optional description for the charge.
    //   org_id_for_branding: An optional organization ID for branding purposes.
    //   disable_confirmation_email: An optional flag to disable the confirmation email.
    //
    // Returns:
    //   A ChargeOrganizationResponse containing the invoice ID.
    viam::app::v1::ChargeOrganizationResponse ChargeOrganization(
        const std::string& org_id_to_charge,
        double subtotal,
        double tax,
        const std::string& description = "",
        const std::string& org_id_for_branding = "",
        bool disable_confirmation_email = false);

    // Deprecated: Use ChargeOrganization instead
    // Directly create a flat fee invoice for an organization and charge on the spot
    //
    // Args:
    //   org_id_to_charge: The organization ID to charge.
    //   amount: The total amount to charge.
    //   description: An optional description for the invoice.
    //   org_id_for_branding: An optional organization ID for branding purposes.
    //   disable_email: An optional flag to disable the confirmation email.
    //
    // Returns:
    //   A CreateInvoiceAndChargeImmediatelyResponse containing the invoice ID.
    //
    // Deprecated: Use ChargeOrganization instead.
    [[deprecated("Use ChargeOrganization instead.")]]
    viam::app::v1::CreateInvoiceAndChargeImmediatelyResponse CreateInvoiceAndChargeImmediately(
        const std::string& org_id_to_charge,
        double amount,
        const std::string& description = "",
        const std::string& org_id_for_branding = "",
        bool disable_email = false);

   private:
    ViamChannel channel_;
    std::unique_ptr<viam::app::v1::BillingService::Stub> stub_;
};

}  // namespace sdk
}  // namespace viam
